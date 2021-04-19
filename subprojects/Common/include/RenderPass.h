#pragma once
#include<Definations.i>
#include<Log.h>
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
        void init(VkDevice* device);
        void terminate();
    protected:
        RenderPass();
        virtual ~RenderPass();
        VkDevice* device;
        VkRenderPass renderPass;
        VkAttachmentDescription* attachments=nullptr;
        VkAttachmentReference* attachmentsRefs=nullptr;
        VkSubpassDescription* subPasses=nullptr;
        VkSubpassDependency* depenedncies=nullptr;
        Framebuffer* framebuffers;
        uint32_t attachmentsCount,attachmentRefsCount,subpassesCount,framebuffersCount;
        virtual void setupAttachments()=0;
        virtual void setupSubPasses()=0;
        virtual void setupFramebuffers()=0;
    };
}