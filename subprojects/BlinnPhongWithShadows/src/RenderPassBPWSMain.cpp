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
#include<RenderPassBPWSMain.h>
using namespace WideOpenBPWS;
RenderPassBPWSMain::RenderPassBPWSMain(){}
RenderPassBPWSMain& RenderPassBPWSMain::instance(){
    static RenderPassBPWSMain renderPass;
    return renderPass;
}
void RenderPassBPWSMain::setupAttachments(){
    attachmentsCount=2;
    attachmentRefsCount=2;
    attachments=new VkAttachmentDescription[2];
    attachmentsRefs=new VkAttachmentReference[2];
    attachments[0].finalLayout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[0].flags=0;
    attachments[0].format=VK_FORMAT_D16_UNORM;
    attachments[0].initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].samples=VK_SAMPLE_COUNT_1_BIT;
    attachments[0].stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].finalLayout=VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[1].flags=0;
    attachments[1].format=renderer->getSwapchainFormat().format;
    attachments[1].initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].samples=VK_SAMPLE_COUNT_1_BIT;
    attachments[1].stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    attachmentsRefs[0].attachment=0;
    attachmentsRefs[0].layout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachmentsRefs[1].attachment=1;
    attachmentsRefs[1].layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}
void RenderPassBPWSMain::setupSubPasses(){
    subPassesCount=1;
    dependenciesCount=1;
    subPasses=new VkSubpassDescription[1];
    depenedncies=new VkSubpassDependency[1];
    subPasses[0].colorAttachmentCount=1;
    subPasses[0].flags=0;
    subPasses[0].inputAttachmentCount=0;
    subPasses[0].pColorAttachments=&attachmentsRefs[1];
    subPasses[0].pDepthStencilAttachment=&attachmentsRefs[0];
    subPasses[0].pipelineBindPoint=VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPasses[0].preserveAttachmentCount=0;
    subPasses[0].pResolveAttachments=nullptr;
    depenedncies[0].dependencyFlags=0;
    depenedncies[0].srcSubpass=VK_SUBPASS_EXTERNAL;
    depenedncies[0].dstSubpass=0;
    depenedncies[0].srcStageMask=VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    depenedncies[0].dstStageMask=VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    depenedncies[0].srcAccessMask=VK_ACCESS_MEMORY_WRITE_BIT;
    depenedncies[0].dstAccessMask=VK_ACCESS_MEMORY_READ_BIT;
}
void RenderPassBPWSMain::setupFramebuffers(){
    framebuffersCount=1;
    framebuffers=new Framebuffer[1];
    framebuffers[0].imagesCount=1;
    framebuffers[0].images=new VkImage[1];
    framebuffers[0].imageViews=new VkImageView[1];
    framebuffers[0].imagesMemory=new VkDeviceMemory[1];
    framebuffers[0].swapchainImage=&renderer->getSwapchainImageViews()[0];
    uint32_t graphicsQueueFamilyIndex=renderer->getGraphicsQueueFamilyIndex();
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.arrayLayers=1;
    imageCreateInfo.extent=renderer->getExtent3D();
    imageCreateInfo.imageType=VK_IMAGE_TYPE_2D;
    imageCreateInfo.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.mipLevels=1;
    imageCreateInfo.pQueueFamilyIndices=&graphicsQueueFamilyIndex;
    imageCreateInfo.queueFamilyIndexCount=1;
    imageCreateInfo.samples=VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.tiling=VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage=VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageCreateInfo.format=VK_FORMAT_D16_UNORM;
    if(vkCreateImage(renderer->getDevice(),&imageCreateInfo,ALLOCATOR,&framebuffers[0].images[0])!=VK_SUCCESS){
        LOG.error("Failed to create depth image for main renderpass");
    }
    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(renderer->getDevice(),framebuffers[0].images[0],&memReq);
    framebuffers[0].imagesMemory[0]=renderer->allocateMemory(memReq,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if(vkBindImageMemory(renderer->getDevice(),framebuffers[0].images[0],framebuffers[0].imagesMemory[0],0)!=VK_SUCCESS){
        LOG.error("Failed to bind memory to image");
    }
    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.format=VK_FORMAT_D16_UNORM;
    imageViewCreateInfo.image=framebuffers[0].images[0];
    imageViewCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_DEPTH_BIT;
    imageViewCreateInfo.subresourceRange.baseArrayLayer=0;
    imageViewCreateInfo.subresourceRange.baseMipLevel=0;
    imageViewCreateInfo.subresourceRange.layerCount=1;
    imageViewCreateInfo.subresourceRange.levelCount=1;
    imageViewCreateInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
    if(vkCreateImageView(renderer->getDevice(),&imageViewCreateInfo,ALLOCATOR,&framebuffers[0].imageViews[0])!=VK_SUCCESS){
        LOG.error("Failed to create image view");
    }
    VkFramebufferCreateInfo framebufferCreateInfo{};
    framebufferCreateInfo.attachmentCount=2;
    framebufferCreateInfo.height=renderer->getExtent2D().height;
    framebufferCreateInfo.layers=1;
    VkImageView imageViews[]={framebuffers[0].imageViews[0],*framebuffers[0].swapchainImage};
    framebufferCreateInfo.pAttachments=imageViews;
    framebufferCreateInfo.renderPass=renderPass;
    framebufferCreateInfo.sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.width=renderer->getExtent2D().width;
    if(vkCreateFramebuffer(renderer->getDevice(),&framebufferCreateInfo,ALLOCATOR,&framebuffers[0].framebuffer)!=VK_SUCCESS){
        LOG.error("Failed to create main framebuffer");
    }
}
void RenderPassBPWSMain::debugRecord(MeshBPWS mesh[2]){
    VkCommandBufferBeginInfo cmdBeginInfo{};
    cmdBeginInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VkRenderPassBeginInfo lightRenderPassBeginInfo{};
    lightRenderPassBeginInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    lightRenderPassBeginInfo.clearValueCount=1;
    VkClearValue clearValue;
    clearValue.depthStencil={1.0f,0};
    lightRenderPassBeginInfo.pClearValues=&clearValue;
    lightRenderPassBeginInfo.renderPass=RenderPassBPWSLight::instance().getRenderPass();
    lightRenderPassBeginInfo.framebuffer=RenderPassBPWSLight::instance().getFrameBuffer(0);
    VkRect2D renderArea;
    renderArea.extent={1024,1024};
    renderArea.offset={0,0};
    lightRenderPassBeginInfo.renderArea=renderArea;
    VkRenderPassBeginInfo mainRenderPassBeingInfo{};
    mainRenderPassBeingInfo.clearValueCount=2;
    mainRenderPassBeingInfo.framebuffer=framebuffers[0].framebuffer;
    VkClearValue mainClearValue[2];
    mainClearValue[0].depthStencil={1.0f,0};
    mainClearValue[1].color={0,0,0};
    mainRenderPassBeingInfo.pClearValues=mainClearValue;
    mainRenderPassBeingInfo.renderPass=renderPass;
    renderArea.extent=renderer->getExtent2D();
    mainRenderPassBeingInfo.renderArea=renderArea;
    mainRenderPassBeingInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    if(vkBeginCommandBuffer(cmdBuffer,&cmdBeginInfo)!=VK_SUCCESS){
        LOG.error("Failed to begin cmd buffer");
    }
    vkCmdBeginRenderPass(cmdBuffer,&lightRenderPassBeginInfo,VK_SUBPASS_CONTENTS_INLINE);
    RenderPassBPWSLight::instance().debugRecord(cmdBuffer,mesh);
    vkCmdEndRenderPass(cmdBuffer);
    vkCmdBeginRenderPass(cmdBuffer,&mainRenderPassBeingInfo,VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmdBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,MainPipeline::instance().getPipeline());
    vkCmdBindDescriptorSets(cmdBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,MainPipeline::instance().getLayout(),0,1,mesh[0].getSecondDescriptorSet(),0,nullptr);
    VkDeviceSize offsets=0;
    vkCmdBindVertexBuffers(cmdBuffer,0,1,mesh[0].getVertexBuffer(),&offsets);
    vkCmdBindIndexBuffer(cmdBuffer,mesh[0].getIndexBuffer(),0,VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer,mesh[0].getIndicesCount(),1,0,0,0);
    vkCmdBindDescriptorSets(cmdBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,MainPipeline::instance().getLayout(),0,1,mesh[1].getSecondDescriptorSet(),0,nullptr);
    vkCmdBindVertexBuffers(cmdBuffer,0,1,mesh[1].getVertexBuffer(),&offsets);
    vkCmdBindIndexBuffer(cmdBuffer,mesh[1].getIndexBuffer(),0,VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer,mesh[1].getIndicesCount(),1,0,0,0);
    vkCmdEndRenderPass(cmdBuffer);
    if(vkEndCommandBuffer(cmdBuffer)!=VK_SUCCESS){
        LOG.error("Failed to record cmd buffer");
    }
}