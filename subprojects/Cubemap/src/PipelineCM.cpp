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
#include<PipelineCM.h>
using namespace WideOpenCM;
using namespace glm;
PipelineCM::PipelineCM(){}
PipelineCM& PipelineCM::instance(){
    static PipelineCM pipeline;
    return pipeline;
}
void PipelineCM::createShaderModules(){
    shaderModulesCount=2;
    shaderModules=new VkShaderModule[2];
    shaderModules[0]=createShaderModule("./Assets/Wide-OpenCM/CubeMapVS.spv");
    shaderModules[1]=createShaderModule("./Assets/Wide-OpenCM/CubeMapFS.spv");
}
void PipelineCM::createLayout(){
    VkDescriptorSetLayout descriptorSetlayout=dsl->getDSL();
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount=1;
    createInfo.pSetLayouts=&descriptorSetlayout;
    if(vkCreatePipelineLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create pipeline layout");
    }
}
void PipelineCM::createShaderStages(){
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
void PipelineCM::createVertexInputState(){
    /*vertex buffer only contains position data,that's why it's only sizeof vec3 stride*/
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
void PipelineCM::createInputAssemblyState(){
    inputAssemblyStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateCreateInfo.primitiveRestartEnable=VK_FALSE;
}
void PipelineCM::createViewportState(){
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
void PipelineCM::createRasterState(){
    rasterStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterStateCreateInfo.depthBiasEnable=VK_FALSE;
    rasterStateCreateInfo.rasterizerDiscardEnable=VK_FALSE;
    rasterStateCreateInfo.polygonMode=VK_POLYGON_MODE_FILL;
    rasterStateCreateInfo.lineWidth=1.0f;
    rasterStateCreateInfo.cullMode=VK_CULL_MODE_NONE;
    rasterStateCreateInfo.frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE;
}
void PipelineCM::createMultisampleState(){
    multisampleStateCreateInfo.rasterizationSamples=VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.sampleShadingEnable=VK_FALSE;
    multisampleStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
}
void PipelineCM::createDepthStencilState(){
    /*depth is off, It's not needed.*/
    depthStencilStateCreateInfo.depthBoundsTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.depthTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.depthWriteEnable=VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable=VK_FALSE;
    depthStencilStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
}
void PipelineCM::createDynamicState(){
    dynamicStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount=0;
}
void PipelineCM::createColorBlendState(){
    attachmentState.colorWriteMask=VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;
    attachmentState.blendEnable=VK_FALSE;
    colorBlendStateCreateInfo.sType=VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable=VK_FALSE;
    colorBlendStateCreateInfo.attachmentCount=1;
    colorBlendStateCreateInfo.pAttachments=&attachmentState;
}