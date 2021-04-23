#pragma once
#include<RenderPass.h>
#include<RenderPassBPWSLight.h>
namespace WideOpenBPWS{
    class RenderPassBPWSMain:public Common::RenderPass{
    private:
        RenderPassBPWSMain();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWSMain& instance();
        void debugRecord();
    };
}