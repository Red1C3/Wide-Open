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
#include<DefinationsBP.i>
#include<vulkan/vulkan.hpp>
#include<GLFW/glfw3.h>
#include<Log.h>
#include<Window.h>
#include<Helpers.h>
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<assimp/scene.h>
#include<DescriptorSetLayoutBP.h>
#define MAX_MESHES 2
namespace WideOpenBP{
    class RendererBP{
    private:
        RendererBP();
        void createInstance();
        /*IF YOUR MACHINE ONLY HAS AN INTEGRATED GPU (THAT SUPPORTS VULKAN) CHANGE THE CONDITION IN THE LOOP TO
        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU*/
        void createPhysicalDevice();
        void createSurface();
        void createDevice();
        void createSwapchain();
        void createDescriptorPool();
        /*ASSUMING THE GRAPHICS QUEUE IS THE SAME AS PRESENT QUEUE*/
        uint32_t queueIndex;
        int queueCount;
        VkInstance vkInstance;
        /*ONLY USES DEDICATED GPUs*/
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkSurfaceKHR surface;
        VkExtent2D swapchainExtent;
        VkSwapchainKHR swapchain;
        VkSurfaceFormatKHR swapchainFormat;
        uint32_t surfaceFormatsCount;
        VkSurfaceFormatKHR* surfaceFormats;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkImageView* swapchainImages;
        uint32_t swapchainImagesCount;
        VkQueue* queues;
        VkCommandPool* cmdPool;
        VkDescriptorPool descriptorPool;
        const char* validationLayer="VK_LAYER_KHRONOS_validation";
        const char** glfwExtensions;
        uint32_t layersCount=1,ExtCount;
        VkPhysicalDeviceMemoryProperties memProperties;
    public:
        /*Assimp importer for meshes importing*/
        static Assimp::Importer importer;
        void init();
        VkDeviceMemory allocateMemory(VkMemoryRequirements memReq,VkMemoryPropertyFlags properties);
        VkDevice& getDevice();
        VkPhysicalDeviceMemoryProperties& getMemProperties();
        VkSurfaceFormatKHR getSwapchainFormat();
        VkImageView* getSwapchainImgViews();
        uint32_t getSwapchainImagesCount();
        VkExtent2D getExtent();
        VkCommandPool getCmdPool();
        VkSwapchainKHR getSwapchain();
        uint32_t getGraphicsQueueIndex();
        void allocateDescriptorSet(VkDescriptorSet* set);
        static RendererBP& instance();
        void terminate();
    };
}