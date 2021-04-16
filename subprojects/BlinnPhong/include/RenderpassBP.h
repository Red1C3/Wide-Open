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