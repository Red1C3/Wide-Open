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
#include<LightPipeline.h>
using namespace WideOpenBPWS;
using namespace glm;
LightPipeline::LightPipeline(){}
void LightPipeline::createShaderModules(){
    shaderModulesCount=1;
    shaderModules=new VkShaderModule[1];
    shaderModules[0]=createShaderModule("./Assets/Wide-OpenBPWS/lightShader.spv");
}
void LightPipeline::createLayout(){
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount=1;
    VkDescriptorSetLayout descriptorSetLayout=dsl->getDSL();
    createInfo.pSetLayouts=&descriptorSetLayout;
    if(vkCreatePipelineLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create light pipeline layout");
    }
}
void LightPipeline::createShaderStages(){
    shaderStagesCount=1;
    shaderStageCreateInfo=new VkPipelineShaderStageCreateInfo[1];
    shaderStageCreateInfo[0]=VkPipelineShaderStageCreateInfo{};
    shaderStageCreateInfo[0].sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[0].module=shaderModules[0];
    shaderStageCreateInfo[0].pName="main";
    shaderStageCreateInfo[0].stage=VK_SHADER_STAGE_VERTEX_BIT;
}
void LightPipeline::createVertexInputState(){
    attribDescs.resize(1);
    bindingDesc.binding=0;
    bindingDesc.inputRate=VK_VERTEX_INPUT_RATE_VERTEX;
    bindingDesc.stride=sizeof(vec3)*2;
    attribDescs[0].location=0;
    attribDescs[0].binding=0;
    attribDescs[0].offset=0;
    attribDescs[0].format=VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions=attribDescs.data();
    vertexInputStateCreateInfo.pVertexBindingDescriptions=&bindingDesc;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount=attribDescs.size();
    vertexInputStateCreateInfo.vertexBindingDescriptionCount=1;
}
void LightPipeline::createInputAssemblyState(){
    inputAssemblyStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.primitiveRestartEnable=VK_FALSE;
    inputAssemblyStateCreateInfo.topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}
void LightPipeline::createViewportState(){
    viewport.height=1024;
    viewport.width=1024;
    viewport.minDepth=0.0f;
    viewport.maxDepth=1.0f;
    viewport.x=0;
    viewport.y=0;
    scissors.extent={1024,1024};
    scissors.offset={0,0};
    viewportStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pScissors=&scissors;
    viewportStateCreateInfo.pViewports=&viewport;
    viewportStateCreateInfo.scissorCount=1;
    viewportStateCreateInfo.viewportCount=1;
}
void LightPipeline::createRasterState(){
    rasterStateCreateInfo.depthClampEnable=VK_FALSE;
    rasterStateCreateInfo.rasterizerDiscardEnable=VK_FALSE;
    rasterStateCreateInfo.polygonMode=VK_POLYGON_MODE_FILL;
    rasterStateCreateInfo.lineWidth=1.0f;
    rasterStateCreateInfo.cullMode=VK_CULL_MODE_BACK_BIT;
    rasterStateCreateInfo.frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterStateCreateInfo.depthBiasEnable=VK_FALSE;
    rasterStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
}
void LightPipeline::createMultisampleState(){
    multisampleStateCreateInfo.rasterizationSamples=VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.sampleShadingEnable=VK_FALSE;
    multisampleStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
}
void LightPipeline::createDepthStencilState(){
    depthStencilStateCreateInfo.depthBoundsTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.depthCompareOp=VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilStateCreateInfo.depthTestEnable=VK_TRUE;
    depthStencilStateCreateInfo.depthWriteEnable=VK_TRUE;
    depthStencilStateCreateInfo.stencilTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
}
void LightPipeline::createColorBlendState(){
    colorBlendStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable=VK_FALSE;
    colorBlendStateCreateInfo.attachmentCount=0;
}
void LightPipeline::createDynamicState(){
    dynamicStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount=0;
}
LightPipeline& LightPipeline::instance(){
    static LightPipeline lightPipeline;
    return lightPipeline;
}