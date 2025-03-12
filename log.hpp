// Illiterate Vulkan!
App::initWindow {
  glfwInit()
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API)
  window = glfwCreateWindow(WIDTH, HEIGHT, "Illiterate Vulkan", nullptr, nullptr)
}
App::initVulkan {
  App::createInstance {
    App::checkValidationLayerSupport {
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data())
      // found validation layer:  VK_LAYER_KHRONOS_validation
    }
    VkInstanceCreateInfo createInfo{}
    App::getRequiredExtensions {
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount)
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data())
      // Available Extensions:  19
      //    VK_KHR_device_group_creation
      //    VK_KHR_external_fence_capabilities
      //    VK_KHR_external_memory_capabilities
      //    VK_KHR_external_semaphore_capabilities
      //    VK_KHR_get_physical_device_properties2
      //    VK_KHR_get_surface_capabilities2
      //    VK_KHR_surface
      //    VK_KHR_win32_surface
      //    VK_EXT_debug_report
      //    VK_EXT_debug_utils
      //    VK_EXT_swapchain_colorspace
      //    VK_KHR_display
      //    VK_KHR_get_display_properties2
      //    VK_KHR_surface_protected_capabilities
      //    VK_EXT_direct_mode_display
      //    VK_EXT_surface_maintenance1
      //    VK_NV_external_memory_capabilities
      //    VK_KHR_portability_enumeration
      //    VK_LUNARG_direct_driver_loading
      // Required Extensions:  3
      //    VK_KHR_surface
      //    VK_KHR_win32_surface
      //    VK_EXT_debug_utils
    }
    // validation layers are enabled
    App::populateDebugMessengerCreateInfo {
    }
    vkCreateInstance(&createInfo, nullptr, &instance)
  }
  App::setupDebugMessenger {
    App::populateDebugMessengerCreateInfo {
    }
    CreateDebugUtilsMessengerEXT {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger)
    }
  }
  App::createSurface {
    glfwCreateWindowSurface(instance, window, nullptr, &surface)
  }
  App::pickPhysicalDevice {
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data())
    // Available Devices:  2
    App::isDeviceSuitable {
      App::findQueueFamilies {
        // queueFamilyCount : 6
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport)
        // found queue families 0 0
      }
      App::checkDeviceExtensionSupport {
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data())
      }
      App::querySwapChainSupport {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities)
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr)
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr)
      }
    }
    // found suitable device  NVIDIA GeForce RTX 3060 Laptop GPU
  }
  App::createLogicalDevice {
    App::findQueueFamilies {
      // queueFamilyCount : 6
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport)
      // found queue families 0 0
    }
    vkCreateDevice(physicalDevice, &createInfo, nullptr, &device)
  }
  App::createSwapChain {
    App::querySwapChainSupport {
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities)
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr)
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr)
    }
    App::chooseSwapSurfaceFormat {
    }
    App::chooseSwapPresentMode {
    }
    App::chooseSwapExtent {
    }
    // swap chain extent:  800 x 600
    // imageUsage: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    App::findQueueFamilies {
      // queueFamilyCount : 6
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport)
      // found queue families 0 0
    }
    // graphics and present queues are the same
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
    createInfo.presentMode = presentMode
    createInfo.oldSwapchain = VK_NULL_HANDLE
    vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain)
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data())
  }
  App::createImageViews {
    swapChainImageViews.resize(swapChainImages.size())
    // swapChainImages.size():  3
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D
    // format:  50
    // components: VK_COMPONENT_SWIZZLE_IDENTITY
    // No mipmapping or multiple layers
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i])
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D
    // format:  50
    // components: VK_COMPONENT_SWIZZLE_IDENTITY
    // No mipmapping or multiple layers
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i])
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D
    // format:  50
    // components: VK_COMPONENT_SWIZZLE_IDENTITY
    // No mipmapping or multiple layers
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i])
  }
  App::createRenderPass {
    // clear the values to a constant at the start
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
    // store the values to memory for reading later
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE
    // not using stencil buffer
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
    // layout transition before and after render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    // which attachment to reference by its index in the attachment descriptions array
    colorAttachmentRef.attachment = 0
    // layout the attachment will have during a subpass
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    // subpass dependencies, for layout transitions
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
    subpass.colorAttachmentCount = 1
    subpass.pColorAttachments = &colorAttachmentRef
    // dependency between the subpass and the external render pass
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL
    dependency.dstSubpass = 0
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    dependency.srcAccessMask = 0
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    // Render Pass
    VkRenderPassCreateInfo renderPassInfo{}
    vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass)
  }
  App::createGraphicsPipeline {
    // Loading shaders, can wither load pre compiled shaders, or compile at runtime to SPIR-V
    readFile {
      // Loading filename: ./bin/shaders/shader.vert.spv fileSize: 1080 bytes
    }
    readFile {
      // Loading filename: ./bin/shaders/shader.frag.spv fileSize: 572 bytes
    }
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode)
    App::createShaderModule {
      vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule)
    }
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode)
    App::createShaderModule {
      vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule)
    }
    // Vertex Input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{}
    Vertex::getBindingDescription {
    }
    Vertex::getAttributeDescriptions {
      attributeDescriptions[0].offset = offsetof(Vertex, pos)
      attributeDescriptions[1].offset = offsetof(Vertex, color)
    }
    // Input Assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{}
    // Viewport State, dynamic states are used for viewport and scissor
    VkPipelineViewportStateCreateInfo viewportState{}
    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{}
    // Using anything else requires enabling GPU features.
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL
    rasterizer.lineWidth = 1.0f
    rasterizer.depthBiasEnable = VK_FALSE
    // Depth Bias, for shadow mapping.
    // Multisampling, disabled for now.
    VkPipelineMultisampleStateCreateInfo multisampling{}
    // Depth and Stencil testing, disabled for now, will pass on nullptr
    // Color Blending, finalColor = newColor * newAlpha <colorBlendOp> oldColor * (1 - newAlpha)
    // It is possible to have multiple color blending attachments, have logical ops, and have separate blending for each color channel.
    VkPipelineColorBlendAttachmentState colorBlendAttachment{}
    VkPipelineColorBlendStateCreateInfo colorBlending{}
    // Setup dynamic states
    // Pipeline Layout, for uniforms and push constants
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{}
    vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout)
    // Create Graphics Pipeline
    // Graphics Pipeline, the final pipeline object that will be used in rendering
    // Here we are combining : shaders, fixed function stages(vertex info, input assembly, viewport syate, rasterizer, multisampleing, depthStencil and color blending), pipeline layout and render pass
    VkGraphicsPipelineCreateInfo pipelineInfo{}
    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline)
    vkDestroyShaderModule(device, vertShaderModule, nullptr)
    vkDestroyShaderModule(device, fragShaderModule, nullptr)
  }
  App::createFrameBuffers {
    swapChainFrameBuffers.resize(swapChainImageViews.size())
    vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i])
    vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i])
    vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i])
  }
  App::createCommandPool {
    App::findQueueFamilies {
      // queueFamilyCount : 6
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport)
      // found queue families 0 0
    }
    VkCommandPoolCreateInfo poolInfo{}
    // Choose graphics family as we are using the command buffer for rendering
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value()
    vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool)
  }
  App::createVertexBuffer {
    // Create Staging Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      // [VALIDATION ERROR] validation layer:  Validation Error: [ VUID-vkGetBufferMemoryRequirements-buffer-parameter ] Object 0: handle = 0x2967bff0a40, type = VK_OBJECT_TYPE_INSTANCE; | MessageID = 0x8001e81c | vkGetBufferMemoryRequirements(): buffer Invalid VkBuffer Object 0xcccccccccccccccc.
The Vulkan spec states: buffer must be a valid VkBuffer handle (https://vulkan.lunarg.com/doc/view/1.4.304.1/windows/antora/spec/latest/chapters/resources.html#VUID-vkGetBufferMemoryRequirements-buffer-parameter)
      // [VALIDATION ERROR] validation layer:  Validation Error: [ UNASSIGNED-Threading-Info ] Object 0: handle = 0xcccccccccccccccc, type = VK_OBJECT_TYPE_BUFFER; | MessageID = 0x5d6b67e2 | vkGetBufferMemoryRequirements(): Couldn't find VkBuffer Object 0xcccccccccccccccc. This should not happen and may indicate a bug in the application.
