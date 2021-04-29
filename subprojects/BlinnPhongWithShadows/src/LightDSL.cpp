#include<LightDSL.h>
using namespace Common;
using namespace WideOpenBPWS;
LightDSL::LightDSL(){}
LightDSL& LightDSL::instance(){
    static LightDSL lightDSL;
    return lightDSL;
}
void LightDSL::init(Renderer* renderer){
    this->renderer=renderer;
    VkDescriptorSetLayoutBinding binding{};
    binding.binding=0;
    binding.descriptorCount=1;
    binding.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags=VK_SHADER_STAGE_ALL;
    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.pBindings=&binding;
    createInfo.bindingCount=1;
    if(vkCreateDescriptorSetLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&descriptorSetLayout)!=VK_SUCCESS){
        LOG.error("Failed to create descriptor set layout for lightDSL");
    }
}
LightDSL::~LightDSL(){}