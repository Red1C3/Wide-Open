#pragma once
#include<Renderer.h>
#include<LightDSL.h>
namespace WideOpenBPWS{
    class RendererBPWS:public Common::Renderer{
    private:
        RendererBPWS();
        void createDescriptorPool();
    public:
        static RendererBPWS& instance();
        void allocateDescriptorSet(VkDescriptorSet* set);
    };
}