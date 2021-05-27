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
        VkCommandBuffer getCmdBuffer();
        VkRenderPass getRenderPass();
        VkFramebuffer getFrameBuffer(uint32_t index);
    protected:
        RenderPass();
        virtual ~RenderPass();
        Renderer* renderer;
        VkRenderPass renderPass;
        VkAttachmentDescription* attachments=nullptr;
        VkAttachmentReference* attachmentsRefs=nullptr;
        VkSubpassDescription* subPasses=nullptr;
        VkSubpassDependency* depenedncies=nullptr;
        VkCommandBuffer cmdBuffer;
        Framebuffer* framebuffers;
        uint32_t attachmentsCount,attachmentRefsCount,subPassesCount,framebuffersCount,dependenciesCount;
        virtual void setupAttachments()=0;
        virtual void setupSubPasses()=0;
        virtual void setupFramebuffers()=0;
    };
}