#pragma once
#include<Renderer.h>
namespace Common{
    class DescriptorSetLayout{
    protected:
        DescriptorSetLayout();
        ~DescriptorSetLayout();
        VkDescriptorSetLayout descriptorSetLayout;
        Renderer* renderer;
        public:
        virtual void init(Renderer* renderer)=0;
        VkDescriptorSetLayout getDSL();
        void terminate();
    };
}