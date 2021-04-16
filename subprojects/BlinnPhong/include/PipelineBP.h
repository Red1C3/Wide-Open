#pragma once
#include<vulkan/vulkan.hpp>
#include<DefinationsBP.i>
#include<Log.h>
#include<RendererBP.h>
#include<Helpers.h>
#include<glm/glm.hpp>
#include<DescriptorSetLayoutBP.h>
#include<RenderpassBP.h>
namespace WideOpenBP{
    class PipelineBP{
    private:
        PipelineBP();
        void createShaderModules();
        void createLayout();
        VkPipeline pipeline;
        VkPipelineLayout layout;
        VkShaderModule modules[2];
    public:
        static PipelineBP& instance();
        VkPipeline getPipeline();
        VkPipelineLayout getPipelineLayout();
        void init();
        void terminate();
    };
}