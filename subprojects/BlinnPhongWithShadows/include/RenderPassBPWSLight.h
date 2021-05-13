#pragma once
#include<RenderPass.h>
#include<LightPipeline.h>
#include<MeshBPWS.h>
namespace WideOpenBPWS{
    class RenderPassBPWSLight:public Common::RenderPass{
    private:
        RenderPassBPWSLight();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWSLight& instance();
        void debugRecord(VkCommandBuffer mainCmdBuffer,MeshBPWS mesh[2]);
        VkImageView getDepthImageView();
    };
}