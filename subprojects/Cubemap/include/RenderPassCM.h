#pragma once
#include<RenderPass.h>
#include<PipelineCM.h>
namespace WideOpenCM{
    class RenderPassCM:public Common::RenderPass{
    private:
        RenderPassCM();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassCM& instance();
        void debugRecord();
    };
}