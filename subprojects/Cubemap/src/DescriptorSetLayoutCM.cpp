#include<DescriptorSetLayoutCM.h>
using namespace WideOpenCM;
using namespace Common;
DescriptorSetLayoutCM::DescriptorSetLayoutCM(){}
DescriptorSetLayoutCM& DescriptorSetLayoutCM::instance(){
    static DescriptorSetLayoutCM dsl;
    return dsl;
}
DescriptorSetLayoutCM::~DescriptorSetLayoutCM(){}
void DescriptorSetLayoutCM::init(Renderer* renderer){
    this->renderer=renderer;
    VkDescriptorSetLayoutBinding bindings[2];
    bindings[0]=VkDescriptorSetLayoutBinding{};
    bindings[1]=VkDescriptorSetLayoutBinding{};
    bindings[0].binding=0;
    bindings[0].descriptorCount=1;
    bindings[0].descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].stageFlags=VK_SHADER_STAGE_ALL;
    bindings[1].binding=1;
    bindings[1].descriptorCount=1;
    bindings[1].descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].stageFlags=VK_SHADER_STAGE_ALL;
    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.bindingCount=2;
    createInfo.pBindings=bindings;
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    if(vkCreateDescriptorSetLayout(renderer->getDevice(),&createInfo,ALLOCATOR,&descriptorSetLayout)!=VK_SUCCESS){
        LOG.error("Failed to create dsl");
    }
}