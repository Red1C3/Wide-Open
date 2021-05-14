#pragma once
#include<vulkan/vulkan.hpp>
#include<GLFW/glfw3.h>
#include<Definations.i>
#include<Log.h>
#include<Helpers.h>
#include<Window.h>
#include<assimp/Importer.hpp>
namespace Common{
    class Renderer{
    protected:
        struct QueueFamily{
            uint32_t count,index;
        };
        Renderer();
        virtual ~Renderer();
        VkInstance vulkanInstance;
        VkPhysicalDevice physicalDevice;
        VkSurfaceKHR surface;
        VkDevice device;
        uint32_t extensionsCount,layersCount=1;
        QueueFamily graphicsQueueFamily,presentQueueFamily;
        VkQueue *graphicsQueues,*presentQueues;
        VkSwapchainKHR swapchain;
        VkImageView* swapchainImageViews;
        uint32_t swapchainImagesCount;
        VkSurfaceFormatKHR swapchainFormat;
        VkExtent2D swapchainExtent;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        VkCommandPool* cmdPool;
        virtual void createDescriptorPool()=0;
        VkDescriptorPool* descriptorPool=nullptr;
        const char* validationLayer="VK_LAYER_KHRONOS_validation";
        const char** glfwExtensions;
        void createInstance();
        void createPhysicalDevice();
        void createSurface();
        void createDevice();
        void createSwapchain();
    public:
        static Assimp::Importer importer;
        VkDeviceMemory allocateMemory(VkMemoryRequirements memReq,VkMemoryPropertyFlags properties);
        VkDevice getDevice();
        VkSurfaceFormatKHR getSwapchainFormat();
        VkExtent2D getExtent2D();
        VkExtent3D getExtent3D();
        uint32_t getGraphicsQueueFamilyIndex();
        VkImageView* getSwapchainImageViews();
        VkCommandPool getCmdPool();
        VkQueue getGraphicsQueue();
        VkSwapchainKHR getSwapchain();
        void init();
        virtual void allocateDescriptorSet(VkDescriptorSet* set)=0;
        void allocateDescriptorSet(VkDescriptorSet* set,VkDescriptorSetLayout* layout);
        void terminate();
    };
}