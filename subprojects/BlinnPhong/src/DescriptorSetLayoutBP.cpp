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
#include<DescriptorSetLayoutBP.h>
using namespace WideOpenBP;
DescriptorSetLayoutBP::DescriptorSetLayoutBP(){}
DescriptorSetLayoutBP& DescriptorSetLayoutBP::instance(){
    static DescriptorSetLayoutBP layout;
    return layout;
}
void DescriptorSetLayoutBP::init(){
    VkDescriptorSetLayoutBinding binding;
    binding.binding=0;
    binding.descriptorCount=1;
    binding.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags=VK_SHADER_STAGE_ALL;
    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.pBindings=&binding;
    createInfo.bindingCount=1;
    if(vkCreateDescriptorSetLayout(DEVICEBP,&createInfo,ALLOCATOR,&layout)!=VK_SUCCESS){
        LOG.error("Failed to create descriptor set layout");
    }
}
VkDescriptorSetLayout DescriptorSetLayoutBP::getLayout(){
    return layout;
}
void DescriptorSetLayoutBP::terminate(){
    vkDestroyDescriptorSetLayout(DEVICEBP,layout,ALLOCATOR);
}