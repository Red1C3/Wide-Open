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
    attachmentsRefs[0].layout=VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
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
    depenedncies[0].srcAccessMask=VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    depenedncies[0].dstAccessMask=VK_ACCESS_SHADER_READ_BIT;
}
void RenderPassBPWSMain::setupFramebuffers(){
    //TODO
}