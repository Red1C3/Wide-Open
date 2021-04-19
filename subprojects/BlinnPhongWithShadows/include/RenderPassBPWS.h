#pragma once
#include<RenderPass.h>
namespace WideOpenBPWS{
    class RenderPassBPWS:public Common::RenderPass{
    private:
        RenderPassBPWS();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassBPWS& instance();
    };
}