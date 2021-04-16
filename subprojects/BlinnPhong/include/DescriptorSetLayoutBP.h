#pragma once
#include<vulkan/vulkan.hpp>
#include<DefinationsBP.i>
#include<RendererBP.h>
#include<Log.h>
namespace WideOpenBP{
    class DescriptorSetLayoutBP{
    private:
        DescriptorSetLayoutBP();
        VkDescriptorSetLayout layout;
    public:
        static DescriptorSetLayoutBP& instance();
        void init();
        VkDescriptorSetLayout getLayout();
        void terminate();
    };
}