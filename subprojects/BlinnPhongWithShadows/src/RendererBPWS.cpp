#include<RendererBPWS.h>
using namespace WideOpenBPWS;
RendererBPWS::RendererBPWS(){}
RendererBPWS& RendererBPWS::instance(){
    static RendererBPWS instance;
    return instance;
}
void RendererBPWS::createDescriptorPool(){
    VkDescriptorPoolSize poolSizes[2];
    poolSizes[0].descriptorCount=4;
    poolSizes[0].type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //TODO change pool size count + add a pool size for samplers
    poolSizes[1].descriptorCount=2;
    poolSizes[1].type=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets=4;
    createInfo.poolSizeCount=2;
    createInfo.pPoolSizes=poolSizes;
    descriptorPool=new VkDescriptorPool;
    if(vkCreateDescriptorPool(getDevice(),&createInfo,ALLOCATOR,descriptorPool)!=VK_SUCCESS){
        LOG.error("Failed to create DescriptorPool");
    }
}
void RendererBPWS::allocateDescriptorSet(VkDescriptorSet* set){
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool=*descriptorPool;
    allocInfo.descriptorSetCount=1;
    VkDescriptorSetLayout layout=LightDSL::instance().getDSL();
    allocInfo.pSetLayouts=&layout;
    if(vkAllocateDescriptorSets(getDevice(),&allocInfo,set)!=VK_SUCCESS){
        LOG.error("Failed to allocate Descriptor Set");
    }
}