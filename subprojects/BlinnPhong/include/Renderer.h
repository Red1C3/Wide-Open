#pragma once
#include<Definations.i>
#include<vulkan/vulkan.hpp>
#include<GLFW/glfw3.h>
#include<Log.h>
#include<Window.h>
namespace WideOpenBP{
    class Renderer{
    private:
        Renderer();
        void createInstance();
        void createPhysicalDevice();
        void createSurface();
        void createDevice();
        void createSwapchain();
        uint32_t queueIndex;
        int queueCount;
        VkInstance vkInstance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkSurfaceKHR surface;
        VkExtent2D swapchainExtent;
        VkSwapchainKHR swapchain;
        VkSurfaceFormatKHR swapchainFormat;
        uint32_t surfaceFormatsCount;
        VkSurfaceFormatKHR* surfaceFormats;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkImageView swapchainImage;
        VkQueue* queues;
        const char* validationLayer="VK_LAYER_KHRONOS_validation";
        const char** glfwExtensions;
        uint32_t layersCount=1,ExtCount;
    public:
        void init();
        static Renderer& instance();
        void terminate();
    };
}