#include<Renderpass.h>
using namespace WideOpenBP;
Renderpass::Renderpass(){}
Renderpass& Renderpass::instance(){
    static Renderpass renderpass;
    return renderpass;
}
void Renderpass::init(){
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
    if(vkCreateRenderPass(DEVICE,&createInfo,ALLOCATOR,&renderpass)!=VK_SUCCESS){
        LOG.error("Failed to create renderpass");
    }
    framebuffersCount=RENDERER.getSwapchainImagesCount();
    framebuffers=new Framebuffer[framebuffersCount];
    createFramebufferImages();
    createFramebuffer();
    createCommandBuffer();
}
void Renderpass::setupAttachments(){
    attachments[0]=VkAttachmentDescription{};
    attachments[0].loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].samples=VK_SAMPLE_COUNT_1_BIT;
    attachments[0].format=RENDERER.getSwapchainFormat().format;
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
void Renderpass::setupSubpass(){
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
void Renderpass::createFramebufferImages(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkImageCreateInfo createInfo{};
        VkImageViewCreateInfo viewInfo{};
        VkMemoryRequirements memReq;
        uint32_t queue=RENDERER.getGraphicsQueueIndex();
        createInfo.arrayLayers=1;
        createInfo.extent.depth=1;
        createInfo.extent.height=RENDERER.getExtent().height;
        createInfo.extent.width=RENDERER.getExtent().width;
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
        if(vkCreateImage(DEVICE,&createInfo,ALLOCATOR,&framebuffers[i].depthImg)!=VK_SUCCESS){
            LOG.error("Failed to create depth img");
        }
        vkGetImageMemoryRequirements(DEVICE,framebuffers[i].depthImg,&memReq);
        framebuffers[i].depthImgMem=RENDERER.allocateMemory(memReq,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if(vkBindImageMemory(DEVICE,framebuffers[i].depthImg,framebuffers[i].depthImgMem,0)!=VK_SUCCESS){
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
        if(vkCreateImageView(DEVICE,&viewInfo,ALLOCATOR,&framebuffers[i].ImageView[1])!=VK_SUCCESS){
            LOG.error("Failed to create depth img view");
        }
        framebuffers[i].ImageView[0]=RENDERER.getSwapchainImgViews()[i];
    }
}
void Renderpass::createFramebuffer(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass=renderpass;
        createInfo.attachmentCount=2;
        createInfo.pAttachments=framebuffers[i].ImageView;
        createInfo.layers=1;
        createInfo.height=WINDOW.getHeight();
        createInfo.width=WINDOW.getWidth();
        if(vkCreateFramebuffer(DEVICE,&createInfo,ALLOCATOR,&framebuffers[i].framebuffer)!=VK_SUCCESS){
            LOG.error("Failed to create framebuffer");
        }
    }
}
void Renderpass::createCommandBuffer(){
    for(uint32_t i=0;i<framebuffersCount;i++){
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.commandBufferCount=1;
        allocInfo.commandPool=RENDERER.getCmdPool();
        allocInfo.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        if(vkAllocateCommandBuffers(DEVICE,&allocInfo,&framebuffers[i].cmdBuffer)!=VK_SUCCESS){
            LOG.error("Failed to create cmd buffer for render pass");
        }
    }
}
void Renderpass::record(){
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
    renderArea.extent=RENDERER.getExtent();
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
void Renderpass::record(VkPipeline pipeline,VkPipelineLayout layout,Mesh& mesh){
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
        renderArea.extent=RENDERER.getExtent();
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
Renderpass::Framebuffer* Renderpass::getFramebuffers(){
    return framebuffers;
}
VkRenderPass Renderpass::getRenderPass(){
    return renderpass;
}
void Renderpass::terminate(){
    vkDestroyRenderPass(DEVICE,renderpass,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyFramebuffer(DEVICE,framebuffers[i].framebuffer,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyImageView(DEVICE,framebuffers[i].ImageView[1],ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkDestroyImage(DEVICE,framebuffers[i].depthImg,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;i++)
        vkFreeMemory(DEVICE,framebuffers[i].depthImgMem,ALLOCATOR);
}