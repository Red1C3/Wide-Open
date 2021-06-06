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
#include<RendererPC.h>
using namespace WideOpenPC;
RendererPC::RendererPC(){}
void RendererPC::createDescriptorPool(){
    /*Descriptor Pool is needed  for historical reasons, sets and pools can't be zero*/
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
/*Won't allocate any descriptor sets, had to be implemented since it's a pure virtual function*/
void RendererPC::allocateDescriptorSet(VkDescriptorSet* set){

}
RendererPC& RendererPC::instance(){
    static RendererPC renderer;
    return renderer;
}