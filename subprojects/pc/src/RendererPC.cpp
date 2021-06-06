#include<RendererPC.h>
using namespace WideOpenPC;
RendererPC::RendererPC(){}
void RendererPC::createDescriptorPool(){
    VkDescriptorPoolSize poolSize{};
    poolSize.descriptorCount=1;
    poolSize.type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets=1;
    createInfo.poolSizeCount=1;
    createInfo.pPoolSizes=&poolSize;
    descriptorPool=new VkDescriptorPool;
    if(vkCreateDescriptorPool(getDevice(),&createInfo,ALLOCATOR,descriptorPool)!=VK_SUCCESS){
        LOG.error("Failed to create descriptor pool");
    }
}
void RendererPC::allocateDescriptorSet(VkDescriptorSet* set){

}
RendererPC& RendererPC::instance(){
    static RendererPC renderer;
    return renderer;
}