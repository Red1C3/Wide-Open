#pragma once
#include<RenderPass.h>
#include<MeshPC.h>
#include<glm/gtc/matrix_transform.hpp>
#include<PipelinePC.h>
namespace WideOpenPC{
    class RenderPassPC:public Common::RenderPass{
    private:
        RenderPassPC();
        void setupAttachments();
        void setupSubPasses();
        void setupFramebuffers();
    public:
        static RenderPassPC& instance();
        void debugRecord(MeshPC mesh);
    };
}