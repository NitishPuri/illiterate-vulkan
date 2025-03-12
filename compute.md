# Illiterate Vulkan

Vulkan tutorial unrolled.

```cpp
// Vulkan Compute!!
App::run {
  App::initWindow {
  }
  App::initVulkan {
    App::createInstance {
      App::checkValidationLayerSupport {
      }
      App::getRequiredExtensions {
      }
      App::populateDebugMessengerCreateInfo {
      }
    }
    App::setupDebugMessenger {
      App::populateDebugMessengerCreateInfo {
      }
      CreateDebugUtilsMessengerEXT {
      }
    }
    App::createSurface {
    }
    App::pickPhysicalDevice {
      App::isDeviceSuitable {
        App::findQueueFamilies {
          vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
          // Need to find queue families with graphics and compute support
          // VK_QUEUE_GRAPHICS_BIT && VK_QUEUE_COMPUTE_BIT
        }
        App::checkDeviceExtensionSupport {
        }
        App::querySwapChainSupport {
        }
      }
    }
    App::createLogicalDevice {
      App::findQueueFamilies {
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
        // Need to find queue families with graphics and compute support
        // VK_QUEUE_GRAPHICS_BIT && VK_QUEUE_COMPUTE_BIT
      }
      vkCreateDevice(physicalDevice, &createInfo, nullptr, &device)
    }
    App::createSwapChain {
      App::querySwapChainSupport {
      }
      App::chooseSwapSurfaceFormat {
      }
      App::chooseSwapPresentMode {
      }
      App::chooseSwapExtent {
      }
      App::findQueueFamilies {
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
        // Need to find queue families with graphics and compute support
        // VK_QUEUE_GRAPHICS_BIT && VK_QUEUE_COMPUTE_BIT
      }
    }
    App::createImageViews {
    }
    App::createRenderPass {
    }
    App::createComputeDescriptorSetLayout {
      vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &computeDescriptorSetLayout)
    }
    App::createGraphicsPipeline {
      App::readFile {
        // Reading file:  ./bin/shaders/compute.vert.spv
      }
      App::readFile {
        // Reading file:  ./bin/shaders/compute.frag.spv
      }
      App::createShaderModule {
      }
      App::createShaderModule {
      }
    }
    App::createComputePipeline {
      App::readFile {
        // Reading file:  ./bin/shaders/compute.comp.spv
      }
      App::createShaderModule {
      }
      // Creating compute pipeline
      computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT
    }
    App::createFramebuffers {
    }
    App::createCommandPool {
      App::findQueueFamilies {
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data())
        // Need to find queue families with graphics and compute support
        // VK_QUEUE_GRAPHICS_BIT && VK_QUEUE_COMPUTE_BIT
      }
    }
    App::createShaderStorageBuffers {
      // Initializing particles positions on a circle.
      VkDeviceSize bufferSize = sizeof(Particle) * PARTICLE_COUNT
      // Create a staging buffer used to upload data to the gpu
      App::createBuffer {
        App::findMemoryType {
        }
      }
      // Copy initial particle data to all storage buffers
      App::createBuffer {
        App::findMemoryType {
        }
      }
      App::copyBuffer {
      }
      App::createBuffer {
        App::findMemoryType {
        }
      }
      App::copyBuffer {
      }
    }
    App::createUniformBuffers {
      App::createBuffer {
        App::findMemoryType {
        }
      }
      App::createBuffer {
        App::findMemoryType {
        }
      }
    }
    App::createDescriptorPool {
    }
    App::createComputeDescriptorSets {
      // Storage buffer info last frame...
      storageBufferInfoLastFrame.buffer = shaderStorageBuffers[(i - 1) % MAX_FRAMES_IN_FLIGHT]
      // Storage buffer for current frame...
      storageBufferInfoCurrentFrame.buffer = shaderStorageBuffers[i]
      // Storage buffer info last frame...
      storageBufferInfoLastFrame.buffer = shaderStorageBuffers[(i - 1) % MAX_FRAMES_IN_FLIGHT]
      // Storage buffer for current frame...
      storageBufferInfoCurrentFrame.buffer = shaderStorageBuffers[i]
    }
    App::createCommandBuffers {
    }
    App::createComputeCommandBuffers {
    }
    App::createSyncObjects {
    }
  }
  App::mainLoop {
    App::drawFrame {
      App::updateUniformBuffer {
      }
      App::recordComputeCommandBuffer {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline)
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout, 0, 1, &computeDescriptorSets[currentFrame], 0, nullptr)
        // Dispatching compute shader!!!
        vkCmdDispatch(commandBuffer, PARTICLE_COUNT / 256, 1, 1)
      }
      // Submitting compute command buffer...
      vkQueueSubmit(computeQueue, 1, &submitInfo, computeInFlightFences[currentFrame])
      App::recordCommandBuffer {
      }
      // Submitting graphics command buffer...
      vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame])
    }
  }
  App::cleanup {
    App::cleanupSwapChain {
    }
    DestroyDebugUtilsMessengerEXT {
    }
  }
}
```
