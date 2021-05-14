#pragma once
#include<vulkan/vulkan.hpp>
#include<glm/glm.hpp>
#include<Renderer.h>
#include<DescriptorSetLayout.h>
namespace Common{
    class Pipeline{
    protected:
        Pipeline();
        virtual ~Pipeline();
        virtual void createShaderModules()=0;
        virtual void createLayout()=0;
        virtual void createShaderStages()=0;
        virtual void createVertexInputState()=0;
        virtual void createInputAssemblyState()=0;
        virtual void createViewportState()=0;
        virtual void createRasterState()=0;
        virtual void createMultisampleState()=0;
        virtual void createDepthStencilState()=0;
        virtual void createColorBlendState()=0;
        virtual void createDynamicState()=0;
        VkShaderModule createShaderModule(const char* path);
        Renderer* renderer;
        DescriptorSetLayout* dsl;
        VkShaderModule* shaderModules;
        uint32_t shaderModulesCount,shaderStagesCount;
        VkPipelineLayout layout;
        VkPipelineShaderStageCreateInfo* shaderStageCreateInfo{};
        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
        VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
        VkPipelineRasterizationStateCreateInfo rasterStateCreateInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        VkVertexInputBindingDescription bindingDesc{};
        VkVertexInputAttributeDescription attribDesc{};
        VkPipelineColorBlendAttachmentState attachmentState{};
        VkViewport viewport{};
        VkRect2D scissors{};
        VkPipeline pipeline;
    public:
        void init(Renderer* renderer,DescriptorSetLayout* dsl,VkRenderPass renderPass,uint32_t subPassIndex);
        VkPipeline getPipeline();
        VkPipelineLayout getLayout();
        void terminate();
    };
}