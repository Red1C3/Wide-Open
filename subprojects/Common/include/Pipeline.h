/*MIT License

Copyright (c) 2021 Mohammad Issawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#pragma once
#include<vulkan/vulkan.hpp>
#include<glm/glm.hpp>
#include<Renderer.h>
#include<DescriptorSetLayout.h>
#include<vector>
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
        std::vector<VkVertexInputAttributeDescription> attribDescs;
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