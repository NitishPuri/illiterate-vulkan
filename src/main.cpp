#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "logger.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

static std::vector<char> readFile(const std::string& filename) {
  LOGFN;
  std::ifstream file{filename, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  LOG("Loading filename:", filename, "fileSize:", fileSize, "bytes");

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

#pragma region VALIDATION_CALLBACK

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"  //
};

const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    // Message is important enough to show
    LOG("[VALIDATION ERROR] validation layer: ", pCallbackData->pMessage);
  } else {
    // Message is not important enough to show
    // LOG("[VALIDATION INFO] validation layer: ", pCallbackData->pMessage);
  }

  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
  LOGFN;
  LOGCALL(auto func =
              (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    LOGCALL(return func(instance, pCreateInfo, pAllocator, pDebugMessenger));
  } else {
    LOGCALL(return VK_ERROR_EXTENSION_NOT_PRESENT);
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
  LOGFN;
  LOGCALL(auto func =
              (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    LOGCALL(func(instance, debugMessenger, pAllocator));
  }
}
#pragma endregion VALIDATION_CALLBACK

class App {
 public:
#pragma region APP
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

 private:
  void initWindow() {
    LOGFN;
    LOGCALL(glfwInit());

    LOGCALL(glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API));
    LOGCALL(glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE));

    LOGCALL(window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));
  }

  void initVulkan() {
    LOGFN;
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
  }

  void mainLoop() {
    LOGFN;
    LOGCALL(while (!glfwWindowShouldClose(window))) { glfwPollEvents(); }
  }

  void cleanup() {
    LOGFN;

    for (auto imageView : swapChainImageViews) {
      LOGCALL(vkDestroyImageView(device, imageView, nullptr));
    }

    LOGCALL(vkDestroySwapchainKHR(device, swapChain, nullptr));
    LOGCALL(vkDestroyDevice(device, nullptr));

    if (enableValidationLayers) {
      DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    LOGCALL(vkDestroySurfaceKHR(instance, surface, nullptr));

    LOGCALL(vkDestroyInstance(instance, nullptr));

    LOGCALL(glfwDestroyWindow(window));
    LOGCALL(glfwTerminate());
  }

#pragma endregion APP

#pragma region VARIABLES
  GLFWwindow* window = nullptr;

  VkInstance instance{};
  VkDebugUtilsMessengerEXT debugMessenger;

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

  VkDevice device;
  VkQueue graphicsQueue;

  VkSurfaceKHR surface;
  VkQueue presentQueue;

  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImageView> swapChainImageViews;
#pragma endregion VARIABLES

#pragma region INSTANCE
  void createInstance() {
    LOGFN;
    if (enableValidationLayers && !checkValidationLayerSupport()) {
      throw std::runtime_error("validation layers requested, but not available!");
    }

    // Optional
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //
    LOGCALL(VkInstanceCreateInfo createInfo{});
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
      LOG("validation layers are enabled");
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();

      populateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;  // callback to
                                                                                 // debug
                                                                                 // instance
                                                                                 // creation
    } else {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
    }

    if (LOGCALL(vkCreateInstance(&createInfo, nullptr, &instance)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }
  }

  std::vector<const char*> getRequiredExtensions() {
    LOGFN;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    LOGCALL(glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount));

    if (true) {
      // query all extensions
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      std::vector<VkExtensionProperties> extensions(extensionCount);
      LOGCALL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()));
      LOG("Available Extensions: ", extensionCount);

      for (const auto& extension : extensions) {
        LOG("  ", extension.extensionName);
      }
    }

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    LOG("Required Extensions: ", extensions.size());
    for (const auto& extension : extensions) {
      LOG("  ", extension);
    }

    return extensions;
  }
#pragma endregion INSTANCE

#pragma region VALIDATION

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    LOGFN;
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
  }

  void setupDebugMessenger() {
    LOGFN;
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
      throw std::runtime_error("failed to set up debug messenger!");
    }
  }

  bool checkValidationLayerSupport() {
    LOGFN;
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    LOGCALL(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

    for (const char* layerName : validationLayers) {
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
          LOG("found validation layer: ", layerName);
          layerFound = true;
          break;
        }
      }

      if (!layerFound) {
        return false;
      }
    }

    return true;
  }

#pragma endregion VALIDATION

#pragma region PHYSICAL_DEVICE

  bool isDeviceSuitable(VkPhysicalDevice device) {
    LOGFN;
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
  }

  bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    LOGFN;
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    LOGCALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()));

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  void pickPhysicalDevice() {
    LOGFN;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    LOGCALL(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));

    LOG("Available Devices: ", deviceCount);

    for (const auto& device : devices) {
      if (isDeviceSuitable(device)) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        LOG("found suitable device ", deviceProperties.deviceName);

        physicalDevice = device;

        break;
      }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("failed to find a suitable GPU!");
    }
  }

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    LOGFN;
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    LOG("queueFamilyCount :", queueFamilyCount);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    LOGCALL(vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()));

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
      VkBool32 presentSupport = false;
      LOGCALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport));
      if (presentSupport) {
        indices.presentFamily = i;
      }

      if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphicsFamily = i;
      }

      if (indices.isComplete()) {
        break;
      }
    }

    LOG("found queue families", indices.graphicsFamily.value(), indices.presentFamily.value());
    return indices;
  }

#pragma endregion PHYSICAL_DEVICE

#pragma region LOGICAL_DEVICE
  void createLogicalDevice() {
    LOGFN;
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;  // yo dawg, we only need one queue

      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (LOGCALL(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create logical device!");
    }

    // get device queue handle
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
  }
#pragma endregion LOGICAL_DEVICE

#pragma region SURFACE
  void createSurface() {
    LOGFN;
    if (LOGCALL(glfwCreateWindowSurface(instance, window, nullptr, &surface)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
  }
#pragma endregion SURFACE

#pragma region SWAPCHAIN
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    LOGFN;
    SwapChainSupportDetails details;

    LOGCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities));

    uint32_t formatCount;
    LOGCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr));
    if (formatCount != 0) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    LOGCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr));
    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    LOGFN;
    for (const auto& availableFormat : availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    LOGFN;
    for (const auto& availablePresentMode : availablePresentModes) {
      // Use VK_PRESENT_MODE_MAILBOX_KHR for triple buffering id enerygy is not a concern
      // Use VK_PRESENT_MODE_FIFO_KHR for double buffering, preferable on mobile devices
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    LOGFN;
    if (capabilities.currentExtent.width != UINT32_MAX) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      LOGCALL(glfwGetFramebufferSize(window, &width, &height));

      LOG("window size: ", WIDTH, "x", HEIGHT);
      LOG("frame buffer size ", width, "x", height);

      VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

      actualExtent.width =
          std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      actualExtent.height =
          std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }

  void createSwapChain() {
    LOGFN;
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // recommended to request at least one more image than the minimum
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;  // always 1, unless Stereo 3D!!
    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // means that we render directly to images in this swapchain
    LOG("imageUsage: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT");

    // for rendering to a separate image first,
    // then copy to the swapchain image, use VK_IMAGE_USAGE_TRANSFER_DST_BIT
    // for post-processing, use VK_IMAGE_USAGE_TRANSFER_SRC_BIT

    // for sharing images between queues
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily != indices.presentFamily) {
      LOG("graphics and present queues are different");
      LOGCALL(createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT);
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      LOG("graphics and present queues are the same");
      LOGCALL(createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE);
      createInfo.queueFamilyIndexCount = 0;      // Optional
      createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    // Used for transformations (eg. 90 degree rotation for mobile devices)
    LOGCALL(createInfo.preTransform = swapChainSupport.capabilities.currentTransform);
    // QUESTION: Can support transparent windows using this flag??
    LOGCALL(createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    LOGCALL(createInfo.presentMode = presentMode);
    // This is window clipping, i.e. if some other window is in front of this one
    // then we can clip the rendering to the visible area
    createInfo.clipped = VK_TRUE;

    // QUESTION: What is the purpose of oldSwapChain?
    LOGCALL(createInfo.oldSwapchain = VK_NULL_HANDLE);

    if (LOGCALL(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    LOGCALL(vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data()));
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
  }

#pragma endregion SWAPCHAIN

#pragma region IMAGE_VIEW
  void createImageViews() {
    LOGFN;
    LOGCALL(swapChainImageViews.resize(swapChainImages.size()));
    LOG("swapChainImages.size(): ", swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = swapChainImages[i];
      LOGCALL(createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D);
      createInfo.format = swapChainImageFormat;
      LOG("format: ", swapChainImageFormat);

      LOG("components: VK_COMPONENT_SWIZZLE_IDENTITY");
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      LOG("No mipmapping or multiple layers");
      LOGCALL(createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      if (LOGCALL(vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i])) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image views!");
      }
    }
  }

#pragma endregion IMAGE_VIEW

#pragma region PIPELINE
  void createGraphicsPipeline() {
    LOGFN;

    auto vertShaderCode = readFile("./bin/shaders/shader.vert.spv");
    auto fragShaderCode = readFile("./bin/shaders/shader.frag.spv");

    LOGCALL(VkShaderModule vertShaderModule = createShaderModule(vertShaderCode));
    LOGCALL(VkShaderModule fragShaderModule = createShaderModule(fragShaderCode));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";  // entry point, whihch means you can control the entry point in the shader
    // vertShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";  // entry point, whihch means you can control the entry point in the shader
    // fragShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
  }

  VkShaderModule createShaderModule(const std::vector<char>& code) {
    LOGFN;
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (LOGCALL(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
  }

#pragma endregion PIPELINE
};

#pragma region MAIN
int main() {
  LOG("Illiterate Vulkan!");
  App app;
  try {
    app.run();
  } catch (const std::exception& e) {
    LOG("[ERROR]", __FUNCTION__, e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
#pragma endregion MAIN