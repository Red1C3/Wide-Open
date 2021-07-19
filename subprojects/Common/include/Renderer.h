/*MIT License

Copyright (c) 2021 Mohammad Issawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Definations.i>
#include <Log.h>
#include <Helpers.h>
#include <Window.h>
#include <assimp/Importer.hpp>
namespace Common
{
    class Renderer
    {
    protected:
        struct QueueFamily
        {
            uint32_t count, index;
        };
        Renderer();
        virtual ~Renderer();
        VkInstance vulkanInstance;
        VkPhysicalDevice physicalDevice;
        VkSurfaceKHR surface;
        VkDevice device;
        uint32_t extensionsCount, layersCount = 1;
        QueueFamily graphicsQueueFamily, presentQueueFamily;
        VkQueue *graphicsQueues, *presentQueues;
        VkSwapchainKHR swapchain;
        VkImageView *swapchainImageViews;
        uint32_t swapchainImagesCount;
        VkSurfaceFormatKHR swapchainFormat;
        VkExtent2D swapchainExtent;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        VkCommandPool *cmdPool;
        virtual void createDescriptorPool() = 0;
        VkDescriptorPool *descriptorPool = nullptr;
        const char *validationLayer = "VK_LAYER_KHRONOS_validation";
        const char **glfwExtensions;
        void createInstance();
        void createPhysicalDevice();
        void createSurface();
        void createDevice();
        void createSwapchain();

    public:
        static Assimp::Importer importer;
        VkDeviceMemory allocateMemory(VkMemoryRequirements memReq, VkMemoryPropertyFlags properties);
        uint32_t getSuitableMemoryTypeIndex(VkMemoryPropertyFlags properties, uint32_t memoryTypeBits);
        VkDevice getDevice();
        VkSurfaceFormatKHR getSwapchainFormat();
        VkExtent2D getExtent2D();
        VkExtent3D getExtent3D();
        uint32_t getGraphicsQueueFamilyIndex();
        VkImageView *getSwapchainImageViews();
        VkCommandPool getCmdPool();
        VkQueue getGraphicsQueue();
        VkSwapchainKHR getSwapchain();
        uint32_t getSwapchainImagesCount();
        void init();
        virtual void allocateDescriptorSet(VkDescriptorSet *set) = 0;
        void allocateDescriptorSet(VkDescriptorSet *set, VkDescriptorSetLayout *layout);
        void terminate();
    };
}