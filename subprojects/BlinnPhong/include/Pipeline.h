#pragma once
#include<vulkan/vulkan.hpp>
#include<Definations.i>
#include<Log.h>
#include<Renderer.h>
#include<Helpers.h>
#include<glm/glm.hpp>
#include<DescriptorSetLayout.h>
#include<Renderpass.h>
namespace WideOpenBP{
    class Pipeline{
    private:
        Pipeline();
        void createShaderModules();
        void createLayout();
        VkPipeline pipeline;
        VkPipelineLayout layout;
        VkShaderModule modules[2];
    public:
        static Pipeline& instance();
        void init();
        void terminate();
    };
}