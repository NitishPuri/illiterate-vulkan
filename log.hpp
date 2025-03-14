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
    // minImageCount:  2
    // recommended to request at least one more image than the minimum
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1
    // imageExtent:  800 x 600
    // imageArrayLayers: 1 // always 1, unless Stereo 3D!!
    // imageUsage: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - we render directly to images in this swapchain
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
  App::createDescriptorLayout {
    vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout)
  }
  App::createGraphicsPipeline {
    // Loading shaders
    readFile {
      // Loading filename: ./bin/shaders/shader.vert.spv fileSize: 1676 bytes
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
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout
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
  App::createTextureImage {
    // ----------------------------------------------------------
    // Create image object backed by device memory.
    // Fill it with pixels from an image file.
    // Create a image sampler.
    // Add a combined image sampler descriptor to sample colors from the texture.
    // We will also see how layout transitions work on images using pipeline barriers.
    // ----------------------------------------------------------

    // Load Image
    stbi_uc* pixels = stbi_load("./res/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha)
    // Create Host Visible Staging Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Copy Image Data to Staging Buffer
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data)
    memcpy(data, pixels, static_cast<size_t>(imageSize))
    vkUnmapMemory(device, stagingBufferMemory)
    // Free Image Data
  }
  App::createVertexBuffer {
    // Create Host Visible Staging Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Copy Vertex data to Staging Buffer
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data)
    memcpy(data, vertices.data(), (size_t)bufferSize)
    vkUnmapMemory(device, stagingBufferMemory)
    // Create Device Local Vertex Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Copy Vertex Data to Staging Buffer
    App::copyBuffer {
      // Command Buffer for Buffer Copy
      vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer)
      // Begin Command Buffer
      vkBeginCommandBuffer(commandBuffer, &beginInfo)
      // Copy Buffer
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion)
      // End Command Buffer
      vkEndCommandBuffer(commandBuffer)
      // Submit Command Buffer
      vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE)
      vkQueueWaitIdle(graphicsQueue)
      // Free Command Buffer
      vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer)
    }
    vkDestroyBuffer(device, stagingBuffer, nullptr)
    vkFreeMemory(device, stagingBufferMemory, nullptr)
  }
  App::createIndexBuffer {
    // Create Host Visible Staging Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Copy Index data to Staging Buffer
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data)
    memcpy(data, indices.data(), (size_t)bufferSize)
    vkUnmapMemory(device, stagingBufferMemory)
    // Create Device Local Index Buffer
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Copy Index Data to Staging Buffer
    App::copyBuffer {
      // Command Buffer for Buffer Copy
      vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer)
      // Begin Command Buffer
      vkBeginCommandBuffer(commandBuffer, &beginInfo)
      // Copy Buffer
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion)
      // End Command Buffer
      vkEndCommandBuffer(commandBuffer)
      // Submit Command Buffer
      vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE)
      vkQueueWaitIdle(graphicsQueue)
      // Free Command Buffer
      vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer)
    }
    vkDestroyBuffer(device, stagingBuffer, nullptr)
    vkFreeMemory(device, stagingBufferMemory, nullptr)
  }
  App::createUniformBuffers {
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Persistently map the buffer memory for uniforms
    // This is a one time operation, and the memory is mapped for the lifetime of the buffer
    vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i])
    App::createBuffer {
      vkCreateBuffer(device, &bufferInfo, nullptr, &buffer)
      // Get Memory Requirements
      App::findMemoryType {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties)
      }
      // Allocate Memory
      vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory)
      // Bind Memory
      vkBindBufferMemory(device, buffer, bufferMemory, 0)
    }
    // Persistently map the buffer memory for uniforms
    // This is a one time operation, and the memory is mapped for the lifetime of the buffer
    vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i])
  }
  App::createDescriptorPool {
    vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool)
  }
  App::createDescriptorSets {
    vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data())
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr)
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr)
  }
  App::createCommandBuffers {
    VkCommandBufferAllocateInfo allocInfo{}
    vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data())
  }
  App::createSyncObjects {
    // Synchronization:
    // Semaphors: signal and wait for the image available and render finished, sync between queues
    // Fences: wait for the frame to finish before starting the next one, sync between CPU and GPU
    // Create fence in signaled state, so that the first frame can start immediately
    // vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore)
    // vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore)
    // vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence)
  }
}
App::mainLoop {
  while (!glfwWindowShouldClose(window))
  App::drawFrame {
    // --------------------------------------------------------------
    // Outline of a frame..
    // Wait for the previous frame to be finished
    // Acquire an image from the swap chain
    // Record a command buffer which draws the scene onto the image.
    // Submit the command buffer to the graphics queue.
    // Present the image to the swap chain for presentation.
    // --------------------------------------------------------------

    // Wait for the previous frame to be finished
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX)
    // Acquire an image from the swap chain
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex)
    vkResetFences(device, 1, &inFlightFences[currentFrame])
    vkResetCommandBuffer(commandBuffers[currentFrame], 0)
    // Record a command buffer which draws the scene onto the image.
    App::recordCommandBuffer {
      vkBeginCommandBuffer(commandBuffer, &beginInfo)
      // Start Render Pass
      VkRenderPassBeginInfo renderPassInfo{}
      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE)
      // Bind Pipeline
      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline)
      // Bind Vertex Buffer
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets)
      // Bind Index Buffer
      vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16)
      // Set dynamic states
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport)
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor)
      // Bind Descriptor Sets
      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr)
      // FINALLY DRAW!!!
      vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0)
      // End Render Pass
      vkCmdEndRenderPass(commandBuffer)
      vkEndCommandBuffer(commandBuffer)
      // Command Buffer Recorded
    }
    // Update Uniform Buffers
    App::updateUniformBuffer {
      UniformBufferObject ubo{}
      ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))
      ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
      ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f)
      // flip the y axis, as glm was designed for OpenGL
      ubo.proj[1][1] *= -1
      memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo))
    }
    // Wait for the imageAvailableSemaphore..
    // Wait till the color attachment is ready for writing..
    // Submit the command buffer to the graphics queue
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame])
    // Presentation
    // Wait for the renderFinishedSemaphore..
    // Specify swap chain to present to.
    // Present the image to the swap chain for presentation.
    result = vkQueuePresentKHR(presentQueue, &presentInfo)
  }
  vkDeviceWaitIdle(device)
}
App::cleanup {
  App::cleanupSwapChain {
    vkDestroyFramebuffer(device, framebuffer, nullptr)
    vkDestroyFramebuffer(device, framebuffer, nullptr)
    vkDestroyFramebuffer(device, framebuffer, nullptr)
    vkDestroyImageView(device, imageView, nullptr)
    vkDestroyImageView(device, imageView, nullptr)
    vkDestroyImageView(device, imageView, nullptr)
    vkDestroySwapchainKHR(device, swapChain, nullptr)
  }
  vkDestroyBuffer(device, uniformBuffers[i], nullptr)
  vkFreeMemory(device, uniformBuffersMemory[i], nullptr)
  vkDestroyBuffer(device, uniformBuffers[i], nullptr)
  vkFreeMemory(device, uniformBuffersMemory[i], nullptr)
  vkDestroyDescriptorPool(device, descriptorPool, nullptr)
  vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr)
  vkDestroyBuffer(device, vertexBuffer, nullptr)
  vkFreeMemory(device, vertexBufferMemory, nullptr)
  vkDestroyBuffer(device, indexBuffer, nullptr)
  vkFreeMemory(device, indexBufferMemory, nullptr)
  vkDestroyPipeline(device, graphicsPipeline, nullptr)
  vkDestroyPipelineLayout(device, pipelineLayout, nullptr)
  vkDestroyRenderPass(device, renderPass, nullptr)
  vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr)
  vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr)
  vkDestroyFence(device, inFlightFences[i], nullptr)
  vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr)
  vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr)
  vkDestroyFence(device, inFlightFences[i], nullptr)
  vkDestroyCommandPool(device, commandPool, nullptr)
  vkDestroyDevice(device, nullptr)
  // [VALIDATION ERROR] validation layer:  Validation Error: [ VUID-vkDestroyDevice-device-05137 ] Object 0: handle = 0x9fde6b0000000014, type = VK_OBJECT_TYPE_BUFFER; | MessageID = 0x4872eaa0 | vkDestroyDevice(): OBJ ERROR : For VkDevice 0x20ef4ef0150[], VkBuffer 0x9fde6b0000000014[] has not been destroyed.
The Vulkan spec states: All child objects created on device must have been destroyed prior to destroying device (https://vulkan.lunarg.com/doc/view/1.4.304.1/windows/antora/spec/latest/chapters/devsandqueues.html#VUID-vkDestroyDevice-device-05137)
  // [VALIDATION ERROR] validation layer:  Validation Error: [ VUID-vkDestroyDevice-device-05137 ] Object 0: handle = 0xdd3a8a0000000015, type = VK_OBJECT_TYPE_DEVICE_MEMORY; | MessageID = 0x4872eaa0 | vkDestroyDevice(): OBJ ERROR : For VkDevice 0x20ef4ef0150[], VkDeviceMemory 0xdd3a8a0000000015[] has not been destroyed.
The Vulkan spec states: All child objects created on device must have been destroyed prior to destroying device (https://vulkan.lunarg.com/doc/view/1.4.304.1/windows/antora/spec/latest/chapters/devsandqueues.html#VUID-vkDestroyDevice-device-05137)
  DestroyDebugUtilsMessengerEXT {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
    func(instance, debugMessenger, pAllocator)
  }
  vkDestroySurfaceKHR(instance, surface, nullptr)
  vkDestroyInstance(instance, nullptr)
  glfwDestroyWindow(window)
  glfwTerminate()
}
