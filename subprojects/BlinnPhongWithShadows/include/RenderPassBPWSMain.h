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
        void debugRecord(MeshBPWS mesh[2]);
    };
}