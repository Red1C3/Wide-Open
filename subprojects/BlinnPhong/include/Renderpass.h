#pragma once
#include<Definations.i>
#include<Renderer.h>
namespace WideOpenBP{
    class Renderpass{
    private:
        Renderpass();
        VkRenderPass renderpass;
        VkAttachmentDescription attachments[2];
        VkAttachmentReference attachmentsRefs[2];
        VkSubpassDescription subpass;
        VkSubpassDependency dependency;
        VkFramebuffer framebuffer;
        VkImageView framebufferImageViews[2];
        VkImage framebufferImage;
        VkDeviceMemory fbImageMem;
        VkCommandBuffer cmdBuffer;
        void setupAttachments();
        void setupSubpass();
        void createFramebuffer();
        void createFramebufferImages();
        void createCommandBuffer();
    public:
        static Renderpass& instance();
        void init();
        void record();
        VkRenderPass getRenderPass();
        VkCommandBuffer getCmdBuffer();
        void terminate();
    };
}