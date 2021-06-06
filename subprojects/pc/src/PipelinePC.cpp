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
#include<PipelinePC.h>
using namespace WideOpenPC;
using namespace Common;
using namespace glm;
using namespace std;
PipelinePC::PipelinePC(){}
PipelinePC& PipelinePC::instance(){
    static PipelinePC pipeline;
    return pipeline;
}
void PipelinePC::createShaderModules(){
    shaderModulesCount=2;
    shaderModules=new VkShaderModule[2];
    shaderModules[0]=createShaderModule("./Assets/Wide-OpenPC/vshader.spv");
    shaderModules[1]=createShaderModule("./Assets/Wide-OpenPC/fshader.spv");
}
void PipelinePC::createLayout(){
    VkPushConstantRange pushConstantsRange{};
    pushConstantsRange.stageFlags=VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantsRange.offset=0;
    pushConstantsRange.size=sizeof(mat4);
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pPushConstantRanges=&pushConstantsRange;
    createInfo.pushConstantRangeCount=1;
    if(vkCreatePipelineLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create Pipeline layout");
    }
}
void PipelinePC::createShaderStages(){
    shaderStagesCount=2;
    shaderStageCreateInfo=new VkPipelineShaderStageCreateInfo[2];
    shaderStageCreateInfo[0]=VkPipelineShaderStageCreateInfo{};
    shaderStageCreateInfo[0].sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[0].stage=VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageCreateInfo[0].module=shaderModules[0];
    shaderStageCreateInfo[0].pName="main";
    shaderStageCreateInfo[1]=VkPipelineShaderStageCreateInfo{};
    shaderStageCreateInfo[1].sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[1].stage=VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStageCreateInfo[1].module=shaderModules[1];
    shaderStageCreateInfo[1].pName="main";
}
void PipelinePC::createVertexInputState(){
    attribDescs.resize(1);
    bindingDesc.binding=0;
    bindingDesc.stride=sizeof(vec3);
    bindingDesc.inputRate=VK_VERTEX_INPUT_RATE_VERTEX;
    attribDescs[0].binding=0;
    attribDescs[0].format=VK_FORMAT_R32G32B32_SFLOAT;
    attribDescs[0].location=0;
    attribDescs[0].offset=0;
    vertexInputStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount=1;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount=1;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions=attribDescs.data();
    vertexInputStateCreateInfo.pVertexBindingDescriptions=&bindingDesc;
}
void PipelinePC::createInputAssemblyState(){
    inputAssemblyStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateCreateInfo.primitiveRestartEnable=VK_FALSE;
}
void PipelinePC::createViewportState(){
    viewport.x=0;
    viewport.y=0;
    viewport.width=renderer->getExtent2D().width;
    viewport.height=renderer->getExtent2D().height;
    viewport.minDepth=0.0f;
    viewport.maxDepth=1.0f;
    scissors.extent=renderer->getExtent2D();
    scissors.offset={0,0};
    viewportStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount=1;
    viewportStateCreateInfo.scissorCount=1;
    viewportStateCreateInfo.pScissors=&scissors;
    viewportStateCreateInfo.pViewports=&viewport;
}
void PipelinePC::createRasterState(){
    rasterStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterStateCreateInfo.depthBiasEnable=VK_FALSE;
    rasterStateCreateInfo.rasterizerDiscardEnable=VK_FALSE;
    rasterStateCreateInfo.polygonMode=VK_POLYGON_MODE_FILL;
    rasterStateCreateInfo.lineWidth=1.0f;
    rasterStateCreateInfo.cullMode=VK_CULL_MODE_NONE;
    rasterStateCreateInfo.frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE;
}
void PipelinePC::createMultisampleState(){
    multisampleStateCreateInfo.rasterizationSamples=VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.sampleShadingEnable=VK_FALSE;
    multisampleStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
}
void PipelinePC::createDepthStencilState(){
    /*depth is off, It's not needed.*/
    depthStencilStateCreateInfo.depthBoundsTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.depthTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.depthWriteEnable=VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
}
void PipelinePC::createDynamicState(){
    dynamicStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount=0;
}
void PipelinePC::createColorBlendState(){
    attachmentState.colorWriteMask=VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;
    attachmentState.blendEnable=VK_FALSE;
    colorBlendStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable=VK_FALSE;
    colorBlendStateCreateInfo.attachmentCount=1;
    colorBlendStateCreateInfo.pAttachments=&attachmentState;
    
}
void PipelinePC::init(Common::Renderer* renderer,VkRenderPass renderPass){
    this->renderer=renderer;
    cacheFile.open("PipelineCache.bin",ios::in|ios::out|ios::binary|ios::ate);
    if(!cacheFile){
        cacheFile.open("PipelineCache.bin",ios::trunc|ios::in|ios::out|ios::binary);
    }else{
        uint32_t cacheSize=cacheFile.tellg();
        cacheFile.seekg(0,ios::beg);
        cacheData.resize(cacheSize);
        cacheFile.read(cacheData.data(),cacheData.size());
    }
    VkPipelineCacheCreateInfo cacheCreateInfo{};
    cacheCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheCreateInfo.initialDataSize=cacheData.size();
    cacheCreateInfo.pInitialData=cacheData.data();
    if(vkCreatePipelineCache(renderer->getDevice(),&cacheCreateInfo,ALLOCATOR,&pipelineCache)!=VK_SUCCESS){
        LOG.error("Failed to create pipeline cache");
    }
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
    createInfo.subpass=0;
    if(vkCreateGraphicsPipelines(renderer->getDevice(),pipelineCache,1,&createInfo,ALLOCATOR,&pipeline)!=VK_SUCCESS){
        LOG.error("Failed to create pipeline");
    }
}
void PipelinePC::terminate(){
    Pipeline::terminate();
    size_t pipelineCacheSize;
    vkGetPipelineCacheData(renderer->getDevice(),pipelineCache,&pipelineCacheSize,nullptr);
    cacheData.resize(pipelineCacheSize);
    vkGetPipelineCacheData(renderer->getDevice(),pipelineCache,&pipelineCacheSize,cacheData.data());
    vkDestroyPipelineCache(renderer->getDevice(),pipelineCache,ALLOCATOR);
    cacheFile.seekp(0,ios::beg);
    cacheFile.write(cacheData.data(),cacheData.size());
    cacheFile.flush();
    cacheFile.close();
}