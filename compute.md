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
        }
        App::checkDeviceExtensionSupport {
        }
        App::querySwapChainSupport {
        }
      }
    }
    App::createLogicalDevice {
      App::findQueueFamilies {
      }
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
      }
    }
    App::createImageViews {
    }
    App::createRenderPass {
    }
    App::createComputeDescriptorSetLayout {
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
    }
    App::createFramebuffers {
    }
    App::createCommandPool {
      App::findQueueFamilies {
      }
    }
    App::createBuffer {
      App::findMemoryType {
      }
    }
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
      }
      App::recordCommandBuffer {
      }
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
