#pragma once
#include<Renderer.h>
namespace WideOpenPC{
    class RendererPC:public Common::Renderer{
    private:
        RendererPC();
        void createDescriptorPool();
    public:
        static RendererPC& instance();
        void allocateDescriptorSet(VkDescriptorSet* set);
    };
}