#pragma once
#include<RenderPass.h>
namespace WideOpenBPWS{
    class RenderPassBPWSMain:public Common::RenderPass{
    private:
        RenderPassBPWSMain();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWSMain& instance();
    };
}