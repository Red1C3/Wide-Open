#include<RenderPass.h>
using namespace Common;
RenderPass::RenderPass(){}
RenderPass::~RenderPass(){}
void RenderPass::init(Renderer* renderer){
    this->renderer=renderer;
    setupAttachments();
    setupSubPasses();
    if(!(attachments&&attachmentsRefs&&subPasses&&depenedncies)){
        LOG.error("Attachments or subpasses were not setup properly");
    }
    VkRenderPassCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount=attachmentsCount;
    createInfo.pAttachments=attachments;
    createInfo.subpassCount=subPassesCount;
    createInfo.pSubpasses=subPasses;
    createInfo.dependencyCount=dependenciesCount;
    createInfo.pDependencies=depenedncies;
    if(vkCreateRenderPass(renderer->getDevice(),&createInfo,ALLOCATOR,&renderPass)!=VK_SUCCESS){
        LOG.error("Failed to create renderpass");
    }
    LOG.log("Created renderpass successfully");
    setupFramebuffers();
    if(!framebuffers){
        LOG.error("Framebuffers were not setup properly");
    }
}
void RenderPass::terminate(){
    vkDestroyRenderPass(renderer->getDevice(),renderPass,ALLOCATOR);
    for(uint32_t i=0;i<framebuffersCount;++i){
        vkDestroyFramebuffer(renderer->getDevice(),framebuffers[i].framebuffer,ALLOCATOR);
        for(uint32_t j=0;j<framebuffers[i].imagesCount;++j){
            vkDestroyImageView(renderer->getDevice(),framebuffers[i].imageViews[j],ALLOCATOR);
            vkDestroyImage(renderer->getDevice(),framebuffers[i].images[j],ALLOCATOR);
            vkFreeMemory(renderer->getDevice(),framebuffers[i].imagesMemory[j],ALLOCATOR);
        }
    }
    delete[] framebuffers;
    delete[] attachments;
    delete[] attachmentsRefs;
    delete[] subPasses;
    delete[] depenedncies;
}