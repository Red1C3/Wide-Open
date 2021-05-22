#include<RendererCM.h>
using namespace WideOpenCM;
RendererCM::RendererCM(){}
RendererCM& RendererCM::instance(){
    static RendererCM renderer;
    return renderer;
}
void RendererCM::createDescriptorPool(){
    VkDescriptorPoolSize poolSizes[2];
    poolSizes[0].descriptorCount=1;
    poolSizes[0].type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].type=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount=1;
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets=2;
    createInfo.poolSizeCount=2;
    createInfo.pPoolSizes=poolSizes;
    descriptorPool=new VkDescriptorPool;
    if(vkCreateDescriptorPool(getDevice(),&createInfo,ALLOCATOR,descriptorPool)!=VK_SUCCESS){
        LOG.error("Failed to create descriptor pool");
    }
}
void RendererCM::allocateDescriptorSet(VkDescriptorSet* set){
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool=*descriptorPool;
    allocInfo.descriptorSetCount=1;
    VkDescriptorSetLayout layout=DescriptorSetLayoutCM::instance().getDSL();
    allocInfo.pSetLayouts=&layout;
    allocInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    if(vkAllocateDescriptorSets(getDevice(),&allocInfo,set)!=VK_SUCCESS){
        LOG.error("Failed to allocate a descriptor set");
    }
}