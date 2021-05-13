#include<MainPipeline.h>
using namespace WideOpenBPWS;
using namespace glm;
MainPipeline::MainPipeline(){}
MainPipeline& MainPipeline::instance(){
    static MainPipeline pipeline;
    return pipeline;
}
void MainPipeline::createShaderModules(){
    shaderModulesCount=2;
    shaderModules=new VkShaderModule[2];
    shaderModules[0]=createShaderModule("./Assets/Wide-OpenBPWS/mainShaderVS.spv");
    shaderModules[1]=createShaderModule("./Assets/Wide-OpenBPWS/mainShaderFS.spv");
}
void MainPipeline::createLayout(){
    VkDescriptorSetLayout descriptorSetLayout=dsl->getDSL();
    VkPipelineLayoutCreateInfo createInfo{
        .sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount=1,
        .pSetLayouts=&descriptorSetLayout
    };
    if(vkCreatePipelineLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create main pipeline layout");
    }
}
void MainPipeline::createShaderStages(){
    shaderStagesCount=2;
    shaderStageCreateInfo=new VkPipelineShaderStageCreateInfo[2];
    shaderStageCreateInfo[0]=VkPipelineShaderStageCreateInfo{
        .sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage=VK_SHADER_STAGE_ALL,
        .module=shaderModules[0],
        .pName="main"
    };
    shaderStageCreateInfo[1]=VkPipelineShaderStageCreateInfo{
        .sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage=VK_SHADER_STAGE_ALL,
        .module=shaderModules[1],
        .pName="main"
    };
}
void MainPipeline::createVertexInputState(){
    bindingDesc={
        .binding=0,
        .stride=sizeof(vec3)*2,
        .inputRate=VK_VERTEX_INPUT_RATE_VERTEX
    };
    attribDesc={
        .location=0,
        .binding=0,
        .format=VK_FORMAT_R32G32B32_SFLOAT,
        .offset=0
    };
    vertexInputStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount=1,
        .pVertexBindingDescriptions=&bindingDesc,
        .vertexAttributeDescriptionCount=1,
        .pVertexAttributeDescriptions=&attribDesc,
    };
}
void MainPipeline::createInputAssemblyState(){
    inputAssemblyStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable=VK_FALSE
    };
}
void MainPipeline::createViewportState(){
    viewport={
        .x=0,
        .y=0, 
        .width=480,
        .height=640,
        .minDepth=0.0f,
        .maxDepth=1.0f,
        
    };
    scissors={
        .offset={0,0},
        .extent={640,480}
    };
    viewportStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount=1, 
        .pViewports=&viewport,
        .scissorCount=1,
        .pScissors=&scissors,
    };
}
void MainPipeline::createRasterState(){
    rasterStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthBiasEnable=VK_FALSE,
        .rasterizerDiscardEnable=VK_FALSE,
        .polygonMode=VK_POLYGON_MODE_FILL,
        .lineWidth=1.0f,
        .cullMode=VK_CULL_MODE_BACK_BIT,
        .frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE,
    };
}
void MainPipeline::createMultisampleState(){
    multisampleStateCreateInfo={
        .rasterizationSamples=VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable=VK_FALSE,
        .sType=VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
    };
}
void MainPipeline::createDepthStencilState(){
    depthStencilStateCreateInfo={
        .depthBoundsTestEnable=VK_FALSE,
        .depthCompareOp=VK_COMPARE_OP_LESS_OR_EQUAL,
        .depthTestEnable=VK_TRUE,
        .depthWriteEnable=VK_TRUE,
        .stencilTestEnable=VK_FALSE,
        .sType=VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
    };
}
void MainPipeline::createDynamicState(){
    dynamicStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount=0
    };
}
void MainPipeline::createColorBlendState(){
    VkPipelineColorBlendAttachmentState attachment{
        .colorWriteMask=VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT,
        .blendEnable=VK_FALSE
    };
    colorBlendStateCreateInfo={
        .sType=VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable=VK_FALSE,
        .attachmentCount=1,
        .pAttachments=&attachment
    };
}