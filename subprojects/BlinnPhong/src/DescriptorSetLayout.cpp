#include<DescriptorSetLayout.h>
using namespace WideOpenBP;
DescriptorSetLayout::DescriptorSetLayout(){}
DescriptorSetLayout& DescriptorSetLayout::instance(){
    static DescriptorSetLayout layout;
    return layout;
}
void DescriptorSetLayout::init(){
    VkDescriptorSetLayoutBinding binding;
    binding.binding=0;
    binding.descriptorCount=1;
    binding.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags=VK_SHADER_STAGE_ALL;
    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.pBindings=&binding;
    createInfo.bindingCount=1;
    if(vkCreateDescriptorSetLayout(DEVICE,&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create descriptor set layout");
    }
}
VkDescriptorSetLayout DescriptorSetLayout::getLayout(){
    return layout;
}
void DescriptorSetLayout::terminate(){
    vkDestroyDescriptorSetLayout(DEVICE,layout,ALLOCATOR);
}