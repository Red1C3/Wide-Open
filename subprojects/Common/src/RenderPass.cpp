#include<RenderPass.h>
using namespace Common;
RenderPass::RenderPass(){}
RenderPass::~RenderPass(){}
void RenderPass::init(VkDevice* device){
    this->device=device;
    setupAttachments();
    setupSubPasses();
    if(!(attachments&&attachmentsRefs&&subPasses&&depenedncies)){
        LOG.error("Attachments or subpasses were not setup properly");
    }
    VkRenderPassCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount=attachmentsCount;
    createInfo.pAttachments=attachments;
    createInfo.subpassCount=subpassesCount;
    createInfo.pSubpasses=subPasses;
    createInfo.dependencyCount=subpassesCount;
    createInfo.pDependencies=depenedncies;
    if(vkCreateRenderPass(*device,&createInfo,ALLOCATOR,&renderPass)!=VK_SUCCESS){
        LOG.error("Failed to create renderpass");
    }
    setupFramebuffers();
    if(!framebuffers){
        LOG.error("Framebuffers were not setup properly");
    }
}
void RenderPass::terminate(){
    //TODO implement
}