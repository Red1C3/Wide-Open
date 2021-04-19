#pragma once
#include<vulkan/vulkan.hpp>
namespace Common{
    class RenderPass{
    public:
        struct Framebuffer{
            VkFramebuffer framebuffer;
            VkImageView* imageViews;
            VkImage* images;
            VkDeviceMemory* imagesMemory;
            VkCommandBuffer cmdBuffer;
            VkImage* swapchainImage=nullptr;
            uint32_t imagesCount;
        };
    protected:
        RenderPass();
        
    };
}