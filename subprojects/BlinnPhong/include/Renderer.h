#pragma once
#include<Definations.i>
#include<vulkan/vulkan.hpp>
#include<GLFW/glfw3.h>
#include<Log.h>
#include<Window.h>
#include<Helpers.h>
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<assimp/scene.h>
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
        VkCommandPool* cmdPool;
        const char* validationLayer="VK_LAYER_KHRONOS_validation";
        const char** glfwExtensions;
        uint32_t layersCount=1,ExtCount;
        VkPhysicalDeviceMemoryProperties memProperties;
    public:
        static Assimp::Importer importer;
        void init();
        VkDeviceMemory allocateMemory(VkMemoryRequirements memReq,VkMemoryPropertyFlags properties);
        VkDevice& getDevice();
        VkPhysicalDeviceMemoryProperties& getMemProperties();
        VkSurfaceFormatKHR getSwapchainFormat();
        VkExtent2D getExtent();
        uint32_t getGraphicsQueueIndex();
        static Renderer& instance();
        void terminate();
    };
}