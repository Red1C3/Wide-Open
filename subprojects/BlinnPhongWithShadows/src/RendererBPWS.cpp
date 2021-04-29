#include<RendererBPWS.h>
using namespace WideOpenBPWS;
RendererBPWS::RendererBPWS(){}
RendererBPWS& RendererBPWS::instance(){
    static RendererBPWS instance;
    return instance;
}
void RendererBPWS::createDescriptorPool(){
    VkDescriptorPoolSize poolSize{};
    poolSize.descriptorCount=2;
    poolSize.type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //TODO change pool size count + add a pool size for samplers
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets=2;
    createInfo.poolSizeCount=1;
    createInfo.pPoolSizes=&poolSize;
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