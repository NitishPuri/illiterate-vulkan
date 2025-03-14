#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

constexpr bool LOG_TO_README = true;
const char* logFileName = LOG_TO_README ? "README.md" : "log.hpp";
#include "logger.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;
const std::string FRAGMENT_SHADER_PATH = "./bin/shaders/frag.spv";
const std::string VERTEX_SHADER_PATH = "./bin/shaders/vert.spv";
// const std::string TEXTURE_PATH = "./res/texture.jpg";
const std::string MODEL_PATH = "./res/viking_room.obj";
const std::string TEXTURE_PATH = "./res/viking_room.png";

std::unordered_set<std::string> OneTimeLogger::loggedFunctions;

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
#pragma region VERTEX_DESC

// Vertex Data
struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  bool operator==(const Vertex& other) const {
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
  }

  static VkVertexInputBindingDescription getBindingDescription() {
    LOGFN;
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    LOGFN;
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    LOGCALL(attributeDescriptions[0].offset = offsetof(Vertex, pos));

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    LOGCALL(attributeDescriptions[1].offset = offsetof(Vertex, color));

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    LOGCALL(attributeDescriptions[2].offset = offsetof(Vertex, texCoord));

    return attributeDescriptions;
  }
};

// const std::vector<Vertex> vertices = {
//     //
//     {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
//     {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
//     {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
//     {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}  //
// };

// const std::vector<Vertex> vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//                                       {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//                                       {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//                                       {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

// const std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//                                       {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//                                       {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//                                       {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

// const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

// const std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//                                       {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//                                       {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//                                       {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

//                                       {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//                                       {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//                                       {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//                                       {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

// const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
           (hash<glm::vec2>()(vertex.texCoord) << 1);
  }
};
}  // namespace std

#pragma endregion VERTEX_DESC

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
    // LOGCALL(glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE));

    LOGCALL(window = glfwCreateWindow(WIDTH, HEIGHT, "Illiterate Vulkan", nullptr, nullptr));
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  }

  static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    app->framebbufferResized = true;
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
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createColorResources();
    createDepthResources();
    createFrameBuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    loadModel();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
  }

  void mainLoop() {
    LOGFN;

    LOGCALL(while (!glfwWindowShouldClose(window))) {
      glfwPollEvents();
      drawFrame();
    }

    LOGCALL(vkDeviceWaitIdle(device));
  }

  void cleanup() {
    LOGFN;

    cleanupSwapChain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      LOGCALL(vkDestroyBuffer(device, uniformBuffers[i], nullptr));
      LOGCALL(vkFreeMemory(device, uniformBuffersMemory[i], nullptr));
    }

    LOGCALL(vkDestroySampler(device, textureSampler, nullptr));
    LOGCALL(vkDestroyImageView(device, textureImageView, nullptr));
    LOGCALL(vkDestroyImage(device, textureImage, nullptr));
    LOGCALL(vkFreeMemory(device, textureImageMemory, nullptr));

    LOGCALL(vkDestroyDescriptorPool(device, descriptorPool, nullptr));
    LOGCALL(vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr));

    LOGCALL(vkDestroyBuffer(device, vertexBuffer, nullptr));
    LOGCALL(vkFreeMemory(device, vertexBufferMemory, nullptr));

    LOGCALL(vkDestroyBuffer(device, indexBuffer, nullptr));
    LOGCALL(vkFreeMemory(device, indexBufferMemory, nullptr));

    LOGCALL(vkDestroyPipeline(device, graphicsPipeline, nullptr));
    LOGCALL(vkDestroyPipelineLayout(device, pipelineLayout, nullptr));

    LOGCALL(vkDestroyRenderPass(device, renderPass, nullptr));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      LOGCALL(vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr));
      LOGCALL(vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr));
      LOGCALL(vkDestroyFence(device, inFlightFences[i], nullptr));
    }

    LOGCALL(vkDestroyCommandPool(device, commandPool, nullptr));

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

  VkRenderPass renderPass;

  VkDescriptorPool descriptorPool;
  VkDescriptorSetLayout descriptorSetLayout;
  std::vector<VkDescriptorSet> descriptorSets;

  VkPipelineLayout pipelineLayout;

  VkPipeline graphicsPipeline;

  std::vector<VkFramebuffer> swapChainFrameBuffers;

  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t mipLevels;
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  std::vector<void*> uniformBuffersMapped;

  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;

  VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

  VkImage colorImage;
  VkDeviceMemory colorImageMemory;
  VkImageView colorImageView;

  bool framebbufferResized = false;

  uint32_t currentFrame = 0;

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

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
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
        msaaSamples = getMaxUsableSampleCount();

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
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

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
    LOG("swap chain extent: ", extent.width, "x", extent.height);

    LOG("minImageCount: ", swapChainSupport.capabilities.minImageCount);
    LOG("recommended to request at least one more image than the minimum");
    LOGCALL(uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1);
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
    LOG("imageExtent: ", extent.width, "x", extent.height);
    LOG("imageArrayLayers: 1 // always 1, unless Stereo 3D!!");
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // means that we render directly to images in this swapchain
    LOG("imageUsage: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT");
    LOG("VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - we render directly to images in this swapchain");

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

  void cleanupSwapChain() {
    LOGFN;

    LOGCALL(vkDestroyImageView(device, colorImageView, nullptr));
    LOGCALL(vkDestroyImage(device, colorImage, nullptr));
    LOGCALL(vkFreeMemory(device, colorImageMemory, nullptr));

    LOGCALL(vkDestroyImageView(device, depthImageView, nullptr));
    LOGCALL(vkDestroyImage(device, depthImage, nullptr));
    LOGCALL(vkFreeMemory(device, depthImageMemory, nullptr));

    for (auto framebuffer : swapChainFrameBuffers) {
      LOGCALL(vkDestroyFramebuffer(device, framebuffer, nullptr));
    }
    for (auto imageView : swapChainImageViews) {
      LOGCALL(vkDestroyImageView(device, imageView, nullptr));
    }
    LOGCALL(vkDestroySwapchainKHR(device, swapChain, nullptr));
  }

  void recreateSwapChain() {
    LOGFN;

    int width = 0, height = 0;
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(window, &width, &height);
      glfwWaitEvents();
    }

    LOGCALL(vkDeviceWaitIdle(device));

    LOGCALL(vkDeviceWaitIdle(device));

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createColorResources();
    createDepthResources();
    createFrameBuffers();
  }

#pragma endregion SWAPCHAIN

#pragma region IMAGE_VIEW
  VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
    LOGFN;
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (LOGCALL(vkCreateImageView(device, &viewInfo, nullptr, &imageView)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image view!");
    }

    return imageView;
  }

  void createImageViews() {
    LOGFN;
    LOGCALL(swapChainImageViews.resize(swapChainImages.size()));
    LOG("swapChainImages.size(): ", swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
      swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
  }

#pragma endregion IMAGE_VIEW

#pragma region PIPELINE
  void createGraphicsPipeline() {
    LOGFN;

    LOG("Loading shaders");
    auto vertShaderCode = readFile(VERTEX_SHADER_PATH);
    auto fragShaderCode = readFile(FRAGMENT_SHADER_PATH);

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

    LOG("Vertex Input");
    LOGCALL(VkPipelineVertexInputStateCreateInfo vertexInputInfo{});
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    LOG("Input Assembly");
    LOGCALL(VkPipelineInputAssemblyStateCreateInfo inputAssembly{});
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;  //
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewports and scissors - if dynamic states are not used
    // LOGCALL(VkViewport viewport{});
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = (float)swapChainExtent.width;
    // viewport.height = (float)swapChainExtent.height;
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;

    // VkRect2D scissor{};
    // scissor.offset = {0, 0};
    // scissor.extent = swapChainExtent;

    LOG("Viewport State, dynamic states are used for viewport and scissor");
    LOGCALL(VkPipelineViewportStateCreateInfo viewportState{});
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr;  // dynamic states
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;  // dynamic states

    // Rasterizer
    LOG("Rasterizer");
    LOGCALL(VkPipelineRasterizationStateCreateInfo rasterizer{});
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    LOG("Using anything else requires enabling GPU features.");
    LOGCALL(rasterizer.polygonMode = VK_POLYGON_MODE_FILL);  // VK_POLYGON_MODE_LINE, VK_POLYGON_MODE_POINT
    LOGCALL(rasterizer.lineWidth = 1.0f);
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;             // VK_CULL_MODE_FRONT_BIT, VK_CULL_MODE_FRONT_AND_BACK
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;  // VK_FRONT_FACE_COUNTER_CLOCKWISE
    LOGCALL(rasterizer.depthBiasEnable = VK_FALSE);
    LOG("Depth Bias, for shadow mapping.");
    rasterizer.depthBiasConstantFactor = 0.0f;  // Optional
    rasterizer.depthBiasClamp = 0.0f;           // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f;     // Optional

    // Multisampling
    LOG("Multisampling, disabled for now.");
    LOGCALL(VkPipelineMultisampleStateCreateInfo multisampling{});
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = msaaSamples;
    multisampling.minSampleShading = .2f;
    multisampling.pSampleMask = nullptr;             // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
    multisampling.alphaToOneEnable = VK_FALSE;       // Optional

    // Depth and Stencil testing
    LOG("Depth and Stencil testing, disabled for now, will pass on nullptr");
    LOGCALL(VkPipelineDepthStencilStateCreateInfo depthStencil{});
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;  // Optional
    depthStencil.maxDepthBounds = 1.0f;  // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};  // Optional
    depthStencil.back = {};   // Optional

    // Color Blending
    LOG("Color Blending, finalColor = newColor * newAlpha <colorBlendOp> oldColor * (1 - newAlpha)");
    LOG("It is possible to have multiple color blending attachments, have logical ops, and have separate blending for "
        "each color channel.");
    LOGCALL(VkPipelineColorBlendAttachmentState colorBlendAttachment{});
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    LOGCALL(VkPipelineColorBlendStateCreateInfo colorBlending{});
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    LOG("Setup dynamic states");
    // std::vector<VkDynamicState> dynamicStates = {
    //     VK_DYNAMIC_STATE_VIEWPORT,           VK_DYNAMIC_STATE_LINE_WIDTH,       VK_DYNAMIC_STATE_DEPTH_BIAS,
    //     VK_DYNAMIC_STATE_BLEND_CONSTANTS,    VK_DYNAMIC_STATE_DEPTH_BOUNDS, VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
    //     VK_DYNAMIC_STATE_STENCIL_WRITE_MASK, VK_DYNAMIC_STATE_STENCIL_REFERENCE};

    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Pipeline layout
    LOG("Pipeline Layout, for uniforms and push constants");
    LOGCALL(VkPipelineLayoutCreateInfo pipelineLayoutInfo{});
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    LOGCALL(pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout);
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (LOGCALL(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout!");
    }

    LOG("Create Graphics Pipeline");
    LOG("Graphics Pipeline, the final pipeline object that will be used in rendering");
    LOG("Here we are combining : shaders, fixed function stages(vertex info, input assembly, viewport syate, "
        "rasterizer, multisampleing, depthStencil and color blending), pipeline layout and render pass");
    LOGCALL(VkGraphicsPipelineCreateInfo pipelineInfo{});
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // Optional
    pipelineInfo.basePipelineIndex = -1;               // Optional

    if (LOGCALL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline)) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create graphics pipeline!");
    }

    // cleanup
    LOGCALL(vkDestroyShaderModule(device, vertShaderModule, nullptr));
    LOGCALL(vkDestroyShaderModule(device, fragShaderModule, nullptr));
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

#pragma region RENDER_PASS
  void createRenderPass() {
    LOGFN;

    LOG("Color Attachment Description");
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = msaaSamples;
    LOG("clear the values to a constant at the start");
    LOGCALL(colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR);
    LOG("store the values to memory for reading later");
    LOGCALL(colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE);
    LOG("not using stencil buffer");
    LOGCALL(colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE);
    LOGCALL(colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
    LOG("layout transition before and after render pass");
    LOGCALL(colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);
    LOG("Transitions to Color attachment optimal for rendering, but cannot be presented directly");
    LOGCALL(colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkAttachmentReference colorAttachmentRef{};
    LOG("which attachment to reference by its index in the attachment descriptions array");
    LOGCALL(colorAttachmentRef.attachment = 0);
    LOG("layout the attachment will have during a subpass");
    LOGCALL(colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    LOG("Depth Attachment Description");
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = msaaSamples;
    LOG("clear the values to a constant at the start");
    LOGCALL(depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR);
    LOG("store the values to memory for reading later");
    LOGCALL(depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
    LOG("not using stencil buffer");
    LOGCALL(depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE);
    LOGCALL(depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
    LOG("layout transition before and after render pass");
    LOGCALL(depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);
    LOGCALL(depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VkAttachmentReference depthAttachmentRef{};
    LOG("which attachment to reference by its index in the attachment descriptions array");
    LOGCALL(depthAttachmentRef.attachment = 1);
    LOG("layout the attachment will have during a subpass");
    LOGCALL(depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    LOG("Resolve Attachment Description");
    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpasses
    LOG("Subpass Description");
    VkSubpassDescription subpass{};
    LOG("subpass dependencies, for layout transitions");
    LOGCALL(subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
    LOGCALL(subpass.colorAttachmentCount = 1);
    LOGCALL(subpass.pColorAttachments = &colorAttachmentRef);
    LOGCALL(subpass.pDepthStencilAttachment = &depthAttachmentRef);
    LOGCALL(subpass.pResolveAttachments = &colorAttachmentResolveRef);

    VkSubpassDependency dependency{};
    LOG("dependency between the subpass and the external render pass");
    LOGCALL(dependency.srcSubpass = VK_SUBPASS_EXTERNAL);
    LOGCALL(dependency.dstSubpass = 0);
    LOGCALL(dependency.srcStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
    LOGCALL(dependency.srcAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    LOGCALL(dependency.dstStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
    LOGCALL(dependency.dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);

    // Render pass
    LOG("Render Pass");
    std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
    LOGCALL(VkRenderPassCreateInfo renderPassInfo{});
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (LOGCALL(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create render pass!");
    }
  }

#pragma endregion RENDER_PASS

#pragma region FRAME_BUFFERS
  void createFrameBuffers() {
    LOGFN;
    LOGCALL(swapChainFrameBuffers.resize(swapChainImageViews.size()));

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
      std::array<VkImageView, 3> attachments = {colorImageView, depthImageView, swapChainImageViews[i]};

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = swapChainExtent.width;
      framebufferInfo.height = swapChainExtent.height;
      framebufferInfo.layers = 1;

      if (LOGCALL(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i])) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  }

#pragma endregion FRAME_BUFFERS

#pragma region VERTEX_BUFFERS

  void loadModel() {
    LOGFN;

    LOG("Load Model");
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
      throw std::runtime_error(warn + err);
    }

    LOG("Vertex Count in model: ", attrib.vertices.size() / 3);
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
      for (const auto& index : shape.mesh.indices) {
        Vertex vertex{};
        vertex.pos = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                      attrib.vertices[3 * index.vertex_index + 2]};

        vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                           1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

        vertex.color = {1.0f, 1.0f, 1.0f};

        if (uniqueVertices.count(vertex) == 0) {
          uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
          vertices.push_back(vertex);
        }

        indices.push_back(uniqueVertices[vertex]);
      }
    }

    LOG("Unique Vertex Count: ", vertices.size());
  }
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,  //
                    VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    LOGFN;
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (LOGCALL(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create buffer!");
    }

    // Memory requirements
    LOG("Get Memory Requirements");
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    LOG("Allocate Memory");
    if (LOGCALL(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate buffer memory!");
    }

    LOG("Bind Memory");
    LOGCALL(vkBindBufferMemory(device, buffer, bufferMemory, 0));
  }

  void createVertexBuffer() {
    LOGFN;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    LOG("Create Host Visible Staging Buffer");
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    LOG("Copy Vertex data to Staging Buffer");
    void* data;
    LOGCALL(vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data));
    LOGCALL(memcpy(data, vertices.data(), (size_t)bufferSize));
    LOGCALL(vkUnmapMemory(device, stagingBufferMemory));

    LOG("Create Device Local Vertex Buffer");
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    LOG("Copy Vertex Data to Staging Buffer");
    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    LOGCALL(vkDestroyBuffer(device, stagingBuffer, nullptr));
    LOGCALL(vkFreeMemory(device, stagingBufferMemory, nullptr));
  }

  void createIndexBuffer() {
    LOGFN;

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    LOG("Create Host Visible Staging Buffer");
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    LOG("Copy Index data to Staging Buffer");
    void* data;
    LOGCALL(vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data));
    LOGCALL(memcpy(data, indices.data(), (size_t)bufferSize));
    LOGCALL(vkUnmapMemory(device, stagingBufferMemory));

    LOG("Create Device Local Index Buffer");
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    LOG("Copy Index Data to Staging Buffer");
    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    LOGCALL(vkDestroyBuffer(device, stagingBuffer, nullptr));
    LOGCALL(vkFreeMemory(device, stagingBufferMemory, nullptr));
  }

  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    LOGFN;

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    LOG("Copy Buffer");
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;  // Optional
    copyRegion.dstOffset = 0;  // Optional
    copyRegion.size = size;
    LOGCALL(vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion));

    endSingleTimeCommands(commandBuffer);
  }

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    LOGFN;
    VkPhysicalDeviceMemoryProperties memProperties;
    LOGCALL(vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties));

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    throw std::runtime_error("failed to find suitable memory type!");
  }

#pragma endregion VERTEX_BUFFERS

#pragma region UNIFORM_BUFFERS

  struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  void createDescriptorSetLayout() {
    LOGFN;

    LOG("Uniform Buffer Object");
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;  // Optional

    LOG("Texture Sampler");
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (LOGCALL(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
    }
  }

  void createDescriptorPool() {
    LOGFN;
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    // poolInfo.poolSizeCount = 1;
    // poolInfo.pPoolSizes = &poolSize;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (LOGCALL(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    }
  }

  void createDescriptorSets() {
    LOGFN;
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (LOGCALL(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data())) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      VkDescriptorImageInfo imageInfo{};
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = textureImageView;
      imageInfo.sampler = textureSampler;

      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = descriptorSets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[1].dstSet = descriptorSets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pImageInfo = &imageInfo;

      LOGCALL(vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
                                     nullptr));
    }
  }

  void createUniformBuffers() {
    LOGFN;
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i],
                   uniformBuffersMemory[i]);

      LOG("Persistently map the buffer memory for uniforms");
      LOG("This is a one time operation, and the memory is mapped for the lifetime of the buffer");
      LOGCALL(vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]));
    }
  }

  void updateUniformBuffer(uint32_t currentImage) {
    LOGFN_ONCE;

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    LOGCALL_ONCE(UniformBufferObject ubo{});
    LOGCALL_ONCE(ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    LOGCALL_ONCE(
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    LOGCALL_ONCE(ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height,
                                             0.1f, 10.0f));
    LOG_ONCE("flip the y axis, as glm was designed for OpenGL");
    LOGCALL_ONCE(ubo.proj[1][1] *= -1);

    LOGCALL_ONCE(memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo)));
  }

#pragma endregion UNIFORM_BUFFERS

#pragma region TEXTURE
  void createTextureImageView() {
    LOGFN;
    textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
  }

  void createTextureSampler() {
    LOGFN;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0;  // Optional
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0;  // Optional

    if (LOGCALL(vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture sampler!");
    }
  }

  void createTextureImage() {
    LOGFN;

    LOG("----------------------------------------------------------");
    LOG("Create image object backed by device memory.");
    LOG("Fill it with pixels from an image file.");
    LOG("Create a image sampler.");
    LOG("Add a combined image sampler descriptor to sample colors from the texture.");
    LOG("We will also see how layout transitions work on images using pipeline barriers.");
    LOG("----------------------------------------------------------\n");

    LOG("Load Image");

    int texWidth, texHeight, texChannels;
    LOGCALL(stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha));
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
      throw std::runtime_error("failed to load texture image!");
    }

    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

    LOG("Create Host Visible Staging Buffer");
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    LOG("Copy Image Data to Staging Buffer");
    void* data;
    LOGCALL(vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data));
    LOGCALL(memcpy(data, pixels, static_cast<size_t>(imageSize)));
    LOGCALL(vkUnmapMemory(device, stagingBufferMemory));

    LOG("Free Image Memory");
    LOGCALL(stbi_image_free(pixels));

    LOG("Create Image");
    LOG("Use the same format as the pixels in the buffer");
    LOG("Tiling optimal for texels accessed in a coherent pattern");
    createImage(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    LOG("Transition Image Layout to Transfer Destination");
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
    LOG("Copy Buffer to Image");
    copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    // LOG("Transition Image Layout to Shader Read Only");
    // transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    //                       VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
    generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);

    LOG("Cleanup");
    LOGCALL(vkDestroyBuffer(device, stagingBuffer, nullptr));
    LOGCALL(vkFreeMemory(device, stagingBufferMemory, nullptr));
  }

  void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
    LOGFN;

    LOG("Check if image format supports linear blitting");
    VkFormatProperties formatProperties;
    LOGCALL(vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties));
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
      throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; ++i) {
      LOGFN_ONCE;
      barrier.subresourceRange.baseMipLevel = i - 1;
      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

      LOGCALL_ONCE(vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                        0, 0, nullptr, 0, nullptr, 1, &barrier));

      VkImageBlit blit{};
      blit.srcOffsets[0] = {0, 0, 0};
      blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
      blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      blit.srcSubresource.mipLevel = i - 1;
      blit.srcSubresource.baseArrayLayer = 0;
      blit.srcSubresource.layerCount = 1;
      blit.dstOffsets[0] = {0, 0, 0};
      blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
      blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      blit.dstSubresource.mipLevel = i;
      blit.dstSubresource.baseArrayLayer = 0;
      blit.dstSubresource.layerCount = 1;

      LOGCALL_ONCE(vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR));

      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      LOGCALL_ONCE(vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier));

      if (mipWidth > 1) mipWidth /= 2;
      if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    LOGCALL(vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                 0, 0, nullptr, 0, nullptr, 1, &barrier));

    endSingleTimeCommands(commandBuffer);
  }

  void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
                   VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                   VkImage& image, VkDeviceMemory& imageMemory) {
    LOGFN;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    LOGCALL(imageInfo.format = format);
    LOGCALL(imageInfo.tiling = tiling);
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    LOGCALL(imageInfo.samples = numSamples);
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    LOG("Flags can be used to specify sparse images, mipmaps, etc.");
    LOGCALL(imageInfo.flags = 0);  // Optional

    if (LOGCALL(vkCreateImage(device, &imageInfo, nullptr, &image)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    LOG("Allocate Memory for Image");
    if (LOGCALL(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory)) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate image memory!");
    }

    LOG("Bind Memory to Image");
    LOGCALL(vkBindImageMemory(device, image, imageMemory, 0));
  }

  void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
                             VkImageAspectFlags aspectMask, uint32_t mipLevels) {
    LOGFN;

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspectMask;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
               newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
               newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask =
          VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
      throw std::invalid_argument("unsupported layout transition!");
    }

    LOG("Pipeline Barrier");
    LOG("Specify the transition to be executed in the command buffer");
    LOGCALL(vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier));

    endSingleTimeCommands(commandBuffer);
  }

  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    LOGFN;

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    LOG("Copy Buffer to Image");
    LOGCALL(vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region));

    endSingleTimeCommands(commandBuffer);
  }
#pragma endregion TEXTURE

#pragma region MSAA
  VkSampleCountFlagBits getMaxUsableSampleCount() {
    LOGFN;

    VkPhysicalDeviceProperties physicalDeviceProperties;
    LOGCALL(vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties));

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) {
      return VK_SAMPLE_COUNT_64_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_32_BIT) {
      return VK_SAMPLE_COUNT_32_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_16_BIT) {
      return VK_SAMPLE_COUNT_16_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_8_BIT) {
      return VK_SAMPLE_COUNT_8_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_4_BIT) {
      return VK_SAMPLE_COUNT_4_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_2_BIT) {
      return VK_SAMPLE_COUNT_2_BIT;
    }

    return VK_SAMPLE_COUNT_1_BIT;
  }

  void createColorResources() {
    LOGFN;

    VkFormat colorFormat = swapChainImageFormat;

    createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
    colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
  }
#pragma endregion MSAA

#pragma region COMMAND_BUFFERS
  void createCommandPool() {
    LOGFN;
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    LOGCALL(VkCommandPoolCreateInfo poolInfo{});
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    LOG("Choose graphics family as we are using the command buffer for rendering");
    LOGCALL(poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value());

    if (LOGCALL(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool)) != VK_SUCCESS) {
      throw std::runtime_error("failed to create command pool!");
    }
  }

  void createCommandBuffers() {
    LOGFN;

    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    LOGCALL(VkCommandBufferAllocateInfo allocInfo{});
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (LOGCALL(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data())) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  }

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    LOGFN_ONCE;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                   // Optional
    beginInfo.pInheritanceInfo = nullptr;  // Optional

    if (LOGCALL_ONCE(vkBeginCommandBuffer(commandBuffer, &beginInfo)) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    LOG_ONCE("Start Render Pass");
    LOGCALL_ONCE(VkRenderPassBeginInfo renderPassInfo{});
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFrameBuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    LOGCALL_ONCE(vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE));

    LOG_ONCE("Bind Pipeline");
    LOGCALL_ONCE(vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline));

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    LOG_ONCE("Bind Vertex Buffer");
    LOGCALL_ONCE(vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets));

    LOG_ONCE("Bind Index Buffer");
    LOGCALL_ONCE(vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32));

    LOG_ONCE("Set dynamic states");
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    LOGCALL_ONCE(vkCmdSetViewport(commandBuffer, 0, 1, &viewport));

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    LOGCALL_ONCE(vkCmdSetScissor(commandBuffer, 0, 1, &scissor));

    LOG_ONCE("Bind Descriptor Sets");
    LOGCALL_ONCE(vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                         &descriptorSets[currentFrame], 0, nullptr));

    LOG_ONCE("FINALLY DRAW!!!");
    // LOGCALL_ONCE(vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0));
    LOGCALL_ONCE(vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0));

    LOG_ONCE("End Render Pass");
    LOGCALL_ONCE(vkCmdEndRenderPass(commandBuffer));

    if (LOGCALL_ONCE(vkEndCommandBuffer(commandBuffer)) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }

    LOG_ONCE("Command Buffer Recorded");
  }

  VkCommandBuffer beginSingleTimeCommands() {
    LOGFN;
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    LOGCALL(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    LOGCALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    return commandBuffer;
  }

  void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    LOGFN;
    LOGCALL(vkEndCommandBuffer(commandBuffer));

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    LOGCALL(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    LOGCALL(vkQueueWaitIdle(graphicsQueue));

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
  }

#pragma endregion COMMAND_BUFFERS

#pragma region DEPTH_BUFFERING
  VkFormat findSupportedFormats(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                VkFormatFeatureFlags features) {
    LOGFN;

    for (VkFormat format : candidates) {
      VkFormatProperties props;
      LOGCALL(vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props));

      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
        return format;
      } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }

    throw std::runtime_error("failed to find supported format!");
  }

  VkFormat findDepthFormat() {
    LOGFN;
    return findSupportedFormats({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

  bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
  }

  void createDepthResources() {
    LOGFN;

    VkFormat depthFormat = findDepthFormat();

    createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
                depthImageMemory);
    depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
  }
#pragma endregion DEPTH_BUFFERING

#pragma region SYNCHRONISATION
  void createSyncObjects() {
    LOGFN;
    LOG("Synchronization:");
    LOG("Semaphors: signal and wait for the image available and render finished, sync between queues");
    LOG("Fences: wait for the frame to finish before starting the next one, sync between CPU and GPU");

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    LOG("Create fence in signaled state, so that the first frame can start immediately");
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    LOG("vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore)");
    LOG("vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore)");
    LOG("vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence)");
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
          vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
          vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization objects for a frame!");
      }
    }
  }
#pragma endregion SYNCHRONISATION

#pragma region DRAW_FRAMES
  void drawFrame() {
    LOGFN_ONCE;
    LOG_ONCE("--------------------------------------------------------------");
    LOG_ONCE("Outline of a frame..");
    LOG_ONCE("Wait for the previous frame to be finished");
    LOG_ONCE("Acquire an image from the swap chain");
    LOG_ONCE("Record a command buffer which draws the scene onto the image.");
    LOG_ONCE("Submit the command buffer to the graphics queue.");
    LOG_ONCE("Present the image to the swap chain for presentation.");
    LOG_ONCE("--------------------------------------------------------------\n");

    LOG_ONCE("Wait for the previous frame to be finished");
    LOGCALL_ONCE(vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX));

    LOG_ONCE("Acquire an image from the swap chain");
    uint32_t imageIndex;
    LOGCALL_ONCE(VkResult result =
                     vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
                                           VK_NULL_HANDLE, &imageIndex));

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Only reset the fence if we are submitting work
    LOGCALL_ONCE(vkResetFences(device, 1, &inFlightFences[currentFrame]));

    LOGCALL_ONCE(vkResetCommandBuffer(commandBuffers[currentFrame], 0));
    LOG_ONCE("Record a command buffer which draws the scene onto the image.");
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    LOG_ONCE("Update Uniform Buffers");
    updateUniformBuffer(currentFrame);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    LOG_ONCE("Wait for the imageAvailableSemaphore..");
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    LOG_ONCE("Wait till the color attachment is ready for writing..");
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    LOG_ONCE("Submit the command buffer to the graphics queue");
    if (LOGCALL_ONCE(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame])) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    // Presentation
    LOG_ONCE("Presentation");
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    LOG_ONCE("Wait for the renderFinishedSemaphore..");
    presentInfo.pWaitSemaphores = signalSemaphores;

    LOG_ONCE("Specify swap chain to present to.");
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;  // Optional

    LOG_ONCE("Present the image to the swap chain for presentation.");
    LOGCALL_ONCE(result = vkQueuePresentKHR(presentQueue, &presentInfo));
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebbufferResized) {
      framebbufferResized = false;
      recreateSwapChain();
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }
#pragma endregion DRAW_FRAMES
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