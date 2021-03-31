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
        int queueIndex;
        int queueCount;
        VkInstance vkInstance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkSurfaceKHR surface;
        const char* validationLayer="VK_LAYER_KHRONOS_validation";
        const char** glfwExtensions;
        uint32_t layersCount=1,ExtCount;
    public:
        void init();
        static Renderer& instance();
        void terminate();
    };
}