#pragma once
#include<Renderer.h>
#include<DescriptorSetLayoutCM.h>
namespace WideOpenCM{
    class RendererCM:public Common::Renderer{
    private:
        RendererCM();
        void createDescriptorPool();
    public:
        static RendererCM& instance();
        void allocateDescriptorSet(VkDescriptorSet* set);
    };
}