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
#include<DescriptorSetLayout.h>
#define MAX_MESHES 2
namespace WideOpenBP{
    class Renderer{
    private:
        Renderer();
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
        static Renderer& instance();
        void terminate();
    };
}