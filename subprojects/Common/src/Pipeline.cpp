#include<Pipeline.h>
using namespace Common;
Pipeline::Pipeline(){}
void Pipeline::init(Renderer* renderer,VkRenderPass renderPass,uint32_t subPassIndex){
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
    createInfo.pMultisampleState=multisampleStateCreateInfo;
    createInfo.pRasterizationState=&rasterStateCreateInfo;
    createInfo.pStages=shaderStageCreateInfo;
    createInfo.pVertexInputState=&vertexInputStateCreateInfo;
    createInfo.pViewportState=&viewportStateCreateInfo;
    createInfo.renderPass=renderPass;
    createInfo.stageCount=shaderStagesCount;
    createInfo.sType=VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.subpass=subPass;
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
}