#include<DescriptorSetLayout.h>
using namespace Common;
DescriptorSetLayout::DescriptorSetLayout(){}
void DescriptorSetLayout::terminate(){
    vkDestroyDescriptorSetLayout(renderer->getDevice(),descriptorSetLayout,ALLOCATOR);
}
DescriptorSetLayout::~DescriptorSetLayout(){}
VkDescriptorSetLayout DescriptorSetLayout::getDSL(){
    return descriptorSetLayout;
}