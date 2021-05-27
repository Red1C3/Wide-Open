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
#include<Pipeline.h>
using namespace Common;
Pipeline::Pipeline(){}
void Pipeline::init(Renderer* renderer,DescriptorSetLayout* dsl,VkRenderPass renderPass,uint32_t subPassIndex){
    this->renderer=renderer;
    this->dsl=dsl;
    createShaderModules();
    createLayout();
    createShaderStages();
    createVertexInputState();
    createInputAssemblyState();
    createViewportState();
    createRasterState();
    createMultisampleState();
    createDepthStencilState();
    createColorBlendState();
    createDynamicState();
    VkGraphicsPipelineCreateInfo createInfo{};
    createInfo.layout=layout;
    createInfo.pColorBlendState=&colorBlendStateCreateInfo;
    createInfo.pDepthStencilState=&depthStencilStateCreateInfo;
    createInfo.pDynamicState=&dynamicStateCreateInfo;
    createInfo.pInputAssemblyState=&inputAssemblyStateCreateInfo;
    createInfo.pMultisampleState=&multisampleStateCreateInfo;
    createInfo.pRasterizationState=&rasterStateCreateInfo;
    createInfo.pStages=shaderStageCreateInfo;
    createInfo.pVertexInputState=&vertexInputStateCreateInfo;
    createInfo.pViewportState=&viewportStateCreateInfo;
    createInfo.renderPass=renderPass;
    createInfo.stageCount=shaderStagesCount;
    createInfo.sType=VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.subpass=subPassIndex;
    if(vkCreateGraphicsPipelines(renderer->getDevice(),VK_NULL_HANDLE,1,&createInfo,ALLOCATOR,&pipeline)!=VK_SUCCESS){
        LOG.error("Failed to create a pipeline");
    }
}
VkShaderModule Pipeline::createShaderModule(const char* path){
    uint32_t shaderSize;
    char* shaderCode=readBinFile(path,shaderSize);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize=shaderSize;
    createInfo.pCode=(uint32_t*) shaderCode;
    VkShaderModule shaderModule;
    if(vkCreateShaderModule(renderer->getDevice(),&createInfo,ALLOCATOR,&shaderModule)!=VK_SUCCESS){
        LOG.error("Failed to create shader module");
    }
    return shaderModule;
}
VkPipeline Pipeline::getPipeline(){
    return pipeline;
}
VkPipelineLayout Pipeline::getLayout(){
    return layout;
}
void Pipeline::terminate(){
    vkDestroyPipeline(renderer->getDevice(),pipeline,ALLOCATOR);
    for(uint32_t i=0;i<shaderModulesCount;++i){
        vkDestroyShaderModule(renderer->getDevice(),shaderModules[i],ALLOCATOR);
    }
    vkDestroyPipelineLayout(renderer->getDevice(),layout,ALLOCATOR);
}
Pipeline::~Pipeline(){}