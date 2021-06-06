#pragma once
#include<RenderPass.h>
namespace WideOpenPC{
    class RenderPassPC:public Common::RenderPass{
    private:
        RenderPassPC();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassPC& instance();
    };
}