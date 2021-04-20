#pragma once
#include<RenderPass.h>
namespace WideOpenBPWS{
    class RenderPassBPWSLight:public Common::RenderPass{
    private:
        RenderPassBPWSLight();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWSLight& instance();
    };
}