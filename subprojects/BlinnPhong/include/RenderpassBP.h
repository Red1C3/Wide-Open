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
#include<DefinationsBP.i>
#include<RendererBP.h>
#include<MeshBP.h>
namespace WideOpenBP{
    class RenderpassBP{
    public:
        struct Framebuffer{
            VkFramebuffer framebuffer;
            VkImageView ImageView[2];
            VkImage depthImg;
            VkDeviceMemory depthImgMem;
            VkCommandBuffer cmdBuffer;
        };
    private:
        RenderpassBP();
        VkRenderPass renderpass;
        VkAttachmentDescription attachments[2];
        VkAttachmentReference attachmentsRefs[2];
        VkSubpassDescription subpass;
        VkSubpassDependency dependency;
        Framebuffer* framebuffers;
        uint32_t framebuffersCount;
        void setupAttachments();
        void setupSubpass();
        void createFramebuffer();
        void createFramebufferImages();
        void createCommandBuffer();
    public:
        static RenderpassBP& instance();
        void init();
        void record();//DEPRACATED
        /*Records commands to command buffers*/
        void record(VkPipeline pipeline,VkPipelineLayout layout,MeshBP& mesh);
        VkRenderPass getRenderPass();
        Framebuffer* getFramebuffers();
        void terminate();
    };
}