#pragma once
#include<Definations.i>
#include<Log.h>
#include<vulkan/vulkan.hpp>
#include<Renderer.h>
namespace Common{
    class RenderPass{
    public:
        struct Framebuffer{
            VkFramebuffer framebuffer;
            VkImageView* imageViews;
            VkImage* images;
            VkDeviceMemory* imagesMemory;
            VkImageView* swapchainImage=nullptr;
            uint32_t imagesCount;
        };
        void init(Renderer* renderer);
        void terminate();
    protected:
        RenderPass();
        virtual ~RenderPass();
        Renderer* renderer;
        VkRenderPass renderPass;
        VkAttachmentDescription* attachments=nullptr;
        VkAttachmentReference* attachmentsRefs=nullptr;
        VkSubpassDescription* subPasses=nullptr;
        VkSubpassDependency* depenedncies=nullptr;
        Framebuffer* framebuffers;
        uint32_t attachmentsCount,attachmentRefsCount,subPassesCount,framebuffersCount,dependenciesCount;
        virtual void setupAttachments()=0;
        virtual void setupSubPasses()=0;
        virtual void setupFramebuffers()=0;
    };
}