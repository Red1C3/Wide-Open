#pragma once
#include<RenderPass.h>
#include<RenderPassBPWSLight.h>
#include<MeshBPWS.h>
#include<MainPipeline.h>
namespace WideOpenBPWS{
    class RenderPassBPWSMain:public Common::RenderPass{
    private:
        RenderPassBPWSMain();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWSMain& instance();
        /*records both render passes into the command buffer of this render pass*/
        void debugRecord(MeshBPWS mesh[2]);
    };
}