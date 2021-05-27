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
#include<RenderpassBP.h>
using namespace WideOpenBP;
RenderpassBP::RenderpassBP(){}
RenderpassBP& RenderpassBP::instance(){
    static RenderpassBP renderpass;
    return renderpass;
}
void RenderpassBP::init(){
    setupAttachments();
    setupSubpass();
    VkRenderPassCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount=2;
    createInfo.pAttachments=attachments;
    createInfo.subpassCount=1;
    createInfo.pSubpasses=&subpass;
    createInfo.dependencyCount=1;
    createInfo.pDependencies=&dependency;
    if(vkCreateRenderPass(DEVICEBP,&createInfo,ALLOCATOR,&renderpass)!=VK_SUCCESS){
        LOG.error("Failed to create renderpass");
    }
    framebuffersCount=RENDERERBP.getSwapchainImagesCount();
    framebuffers=new Framebuffer[framebuffersCount];
    createFramebufferImages();
    createFramebuffer();
    createCommandBuffer();
}
void RenderpassBP::setupAttachments(){
    attachments[0]=VkAttachmentDescription{};
    attachments[0].loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].samples=VK_SAMPLE_COUNT_1_BIT;
    attachments[0].format=RENDERERBP.getSwapchainFormat().format;
    attachments[0].initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout=VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[1]=VkAttachmentDescription{};
    attachments[1].loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].samples=VK_SAMPLE_COUNT_1_BIT;
    attachments[1].format=VK_FORMAT_D16_UNORM;
    attachments[1].initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachmentsRefs[0]=VkAttachmentReference{};
    attachmentsRefs[0].attachment=0;
    attachmentsRefs[0].layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentsRefs[1]=VkAttachmentReference{};
    attachmentsRefs[1].attachment=1;
    attachmentsRefs[1].layout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}
void RenderpassBP::setupSubpass(){
    subpass=VkSubpassDescription{};
    subpass.colorAttachmentCount=1;
    subpass.pColorAttachments=&attachmentsRefs[0];
    subpass.pDepthStencilAttachment=&attachmentsRefs[1];
    subpass.pipelineBindPoint=VK_PIPELINE_BIND_POINT_GRAPHICS;
    dependency=VkSubpassDependency{};
    dependency.srcSubpass=VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass=0;
    dependency.srcStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask=0;
    dependency.dstAccessMask=VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
}
void RenderpassBP::createFramebufferImages(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkImageCreateInfo createInfo{};
        VkImageViewCreateInfo viewInfo{};
        VkMemoryRequirements memReq;
        uint32_t queue=RENDERERBP.getGraphicsQueueIndex();
        createInfo.arrayLayers=1;
        createInfo.extent.depth=1;
        createInfo.extent.height=RENDERERBP.getExtent().height;
        createInfo.extent.width=RENDERERBP.getExtent().width;
        createInfo.format=VK_FORMAT_D16_UNORM;
        createInfo.imageType=VK_IMAGE_TYPE_2D;
        createInfo.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.mipLevels=1;
        createInfo.pQueueFamilyIndices=&queue;
        createInfo.queueFamilyIndexCount=1;
        createInfo.samples=VK_SAMPLE_COUNT_1_BIT;
        createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
        createInfo.sType=VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.tiling=VK_IMAGE_TILING_OPTIMAL;
        createInfo.usage=VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        if(vkCreateImage(DEVICEBP,&createInfo,ALLOCATOR,&framebuffers[i].depthImg)!=VK_SUCCESS){
            LOG.error("Failed to create depth img");
        }
        vkGetImageMemoryRequirements(DEVICEBP,framebuffers[i].depthImg,&memReq);
        framebuffers[i].depthImgMem=RENDERERBP.allocateMemory(memReq,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if(vkBindImageMemory(DEVICEBP,framebuffers[i].depthImg,framebuffers[i].depthImgMem,0)!=VK_SUCCESS){
            LOG.error("Failed to bind memory to depth img");
        }
        viewInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.format=VK_FORMAT_D16_UNORM;
        viewInfo.image=framebuffers[i].depthImg;
        viewInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseArrayLayer=0;
        viewInfo.subresourceRange.baseMipLevel=0;
        viewInfo.subresourceRange.layerCount=1;
        viewInfo.subresourceRange.levelCount=1;
        viewInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
        if(vkCreateImageView(DEVICEBP,&viewInfo,ALLOCATOR,&framebuffers[i].ImageView[1])!=VK_SUCCESS){
            LOG.error("Failed to create depth img view");
        }
        framebuffers[i].ImageView[0]=RENDERERBP.getSwapchainImgViews()[i];
    }
}
void RenderpassBP::createFramebuffer(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass=renderpass;
        createInfo.attachmentCount=2;
        createInfo.pAttachments=framebuffers[i].ImageView;
        createInfo.layers=1;
        createInfo.height=WINDOW.getHeight();
        createInfo.width=WINDOW.getWidth();
        if(vkCreateFramebuffer(DEVICEBP,&createInfo,ALLOCATOR,&framebuffers[i].framebuffer)!=VK_SUCCESS){
            LOG.error("Failed to create framebuffer");
        }
    }
}
void RenderpassBP::createCommandBuffer(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.commandBufferCount=1;
        allocInfo.commandPool=RENDERERBP.getCmdPool();
        allocInfo.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        if(vkAllocateCommandBuffers(DEVICEBP,&allocInfo,&framebuffers[i].cmdBuffer)!=VK_SUCCESS){
            LOG.error("Failed to create cmd buffer for render pass");
        }
    }
}
void RenderpassBP::record(){
    /*
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VkRenderPassBeginInfo renderBeginInfo{};
    renderBeginInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderBeginInfo.clearValueCount=2;
    VkClearValue clearValue[2];
    clearValue[0].color={1,0,0};
    clearValue[1].depthStencil={1.0f,0};
    renderBeginInfo.pClearValues=clearValue;
    renderBeginInfo.framebuffer=framebuffer;
    renderBeginInfo.renderPass=renderpass;
    VkRect2D renderArea;
    renderArea.extent=RENDERERBP.getExtent();
    renderArea.offset={0,0};
    renderBeginInfo.renderArea=renderArea;
    if(vkBeginCommandBuffer(cmdBuffer,&beginInfo)!=VK_SUCCESS){
        LOG.error("Failed to begin cmd buffer");
    }
    vkCmdBeginRenderPass(cmdBuffer,&renderBeginInfo,VK_SUBPASS_CONTENTS_INLINE);
    vkCmdEndRenderPass(cmdBuffer);
    if(vkEndCommandBuffer(cmdBuffer)!=VK_SUCCESS){
        LOG.error("Error occured while recording");
    }
    */
}
void RenderpassBP::record(VkPipeline pipeline,VkPipelineLayout layout,MeshBP& mesh){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VkRenderPassBeginInfo renderBeginInfo{};
        renderBeginInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderBeginInfo.clearValueCount=2;
        VkClearValue clearValue[2];
        clearValue[0].color={0,0,0};
        clearValue[1].depthStencil={1.0f,0};
        renderBeginInfo.pClearValues=clearValue;
        renderBeginInfo.framebuffer=framebuffers[i].framebuffer;
        renderBeginInfo.renderPass=renderpass;
        VkRect2D renderArea;
        renderArea.extent=RENDERERBP.getExtent();
        renderArea.offset={0,0};
        renderBeginInfo.renderArea=renderArea;
        if(vkBeginCommandBuffer(framebuffers[i].cmdBuffer,&beginInfo)!=VK_SUCCESS){
            LOG.error("Failed to begin cmd buffer");
        }
        vkCmdBeginRenderPass(framebuffers[i].cmdBuffer,&renderBeginInfo,VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(framebuffers[i].cmdBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
        VkDescriptorSet set=mesh.getDescriptorSet();
        vkCmdBindDescriptorSets(framebuffers[i].cmdBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,layout,0,1,&set,0,nullptr);
        VkDeviceSize offsets=0;
        vkCmdBindVertexBuffers(framebuffers[i].cmdBuffer,0,1,mesh.getVertexBuffer(),&offsets);
        vkCmdBindIndexBuffer(framebuffers[i].cmdBuffer,mesh.getIndexBuffer(),0,VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(framebuffers[i].cmdBuffer,mesh.getIndicesCount(),1,0,0,0);
        vkCmdEndRenderPass(framebuffers[i].cmdBuffer);
        if(vkEndCommandBuffer(framebuffers[i].cmdBuffer)!=VK_SUCCESS){
            LOG.error("Error occured while recording");
        }
    }
}
RenderpassBP::Framebuffer* RenderpassBP::getFramebuffers(){
    return framebuffers;
}
VkRenderPass RenderpassBP::getRenderPass(){
    return renderpass;
}
void RenderpassBP::terminate(){
    vkDestroyRenderPass(DEVICEBP,renderpass,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyFramebuffer(DEVICEBP,framebuffers[i].framebuffer,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyImageView(DEVICEBP,framebuffers[i].ImageView[1],ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyImage(DEVICEBP,framebuffers[i].depthImg,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkFreeMemory(DEVICEBP,framebuffers[i].depthImgMem,ALLOCATOR);
}