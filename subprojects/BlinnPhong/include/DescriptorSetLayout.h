#pragma once
#include<vulkan/vulkan.hpp>
#include<Definations.i>
#include<Renderer.h>
#include<Log.h>
namespace WideOpenBP{
    class DescriptorSetLayout{
    private:
        DescriptorSetLayout();
        VkDescriptorSetLayout layout;
    public:
        static DescriptorSetLayout& instance();
        void init();
        VkDescriptorSetLayout getLayout();
        void terminate();
    };
}