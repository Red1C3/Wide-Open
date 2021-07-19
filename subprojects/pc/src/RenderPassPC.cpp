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
#include <RenderPassPC.h>
using namespace WideOpenPC;
using namespace glm;
RenderPassPC::RenderPassPC() {}
RenderPassPC &RenderPassPC::instance()
{
    static RenderPassPC rp;
    return rp;
}
void RenderPassPC::setupAttachments()
{
    attachmentsCount = 1;
    attachmentRefsCount = 1;
    attachments = new VkAttachmentDescription[1];
    attachmentsRefs = new VkAttachmentReference[1];
    attachments->finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments->flags = 0;
    attachments->format = renderer->getSwapchainFormat().format;
    attachments->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments->loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments->samples = VK_SAMPLE_COUNT_1_BIT;
    attachments->stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments->storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentsRefs->attachment = 0;
    attachmentsRefs->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}
void RenderPassPC::setupSubPasses()
{
    subPassesCount = 1;
    dependenciesCount = 1;
    subPasses = new VkSubpassDescription[1];
    depenedncies = new VkSubpassDependency[1];
    subPasses->colorAttachmentCount = 1;
    subPasses->flags = 0;
    subPasses->inputAttachmentCount = 0;
    subPasses->pColorAttachments = attachmentsRefs;
    subPasses->pDepthStencilAttachment = nullptr;
    subPasses->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPasses->preserveAttachmentCount = 0;
    subPasses->pResolveAttachments = nullptr;
    depenedncies->dependencyFlags = 0;
    depenedncies->srcSubpass = VK_SUBPASS_EXTERNAL;
    depenedncies->dstSubpass = 0;
    depenedncies->srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    depenedncies->dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    depenedncies->srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
    depenedncies->dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
}
void RenderPassPC::setupFramebuffers()
{
    framebuffersCount = renderer->getSwapchainImagesCount();
    framebuffers = new Framebuffer[framebuffersCount];
    for (uint32_t i = 0; i < framebuffersCount; ++i)
    {
        framebuffers[i].imagesCount = 0;
        framebuffers[i].swapchainImage = &renderer->getSwapchainImageViews()[i];
        VkFramebufferCreateInfo createInfo{};
        createInfo.attachmentCount = 1;
        createInfo.height = renderer->getExtent2D().height;
        createInfo.layers = 1;
        createInfo.pAttachments = framebuffers[i].swapchainImage;
        createInfo.renderPass = renderPass;
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.width = renderer->getExtent2D().width;
        if (vkCreateFramebuffer(renderer->getDevice(), &createInfo, ALLOCATOR, &(framebuffers[i].framebuffer)) != VK_SUCCESS)
        {
            LOG.error("Failed to create framebuffer");
        }
    }
}
void RenderPassPC::debugRecord(MeshPC mesh)
{
    for (uint32_t i = 0; i < renderer->getSwapchainImagesCount(); ++i)
    {
        VkCommandBufferBeginInfo cmdBeginInfo{};
        cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.framebuffer = framebuffers[i].framebuffer;
        VkClearValue clearValue;
        clearValue.color = {1, 0, 0};
        renderPassBeginInfo.pClearValues = &clearValue;
        renderPassBeginInfo.renderPass = renderPass;
        VkRect2D renderArea;
        renderArea.offset = {0, 0};
        renderArea.extent = renderer->getExtent2D();
        renderPassBeginInfo.renderArea = renderArea;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        mat4 persp = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        persp[1][1] *= -1;
        mat4 MVP = persp * lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 0, 1));
        if (vkBeginCommandBuffer(cmdBuffers[i], &cmdBeginInfo) != VK_SUCCESS)
        {
            LOG.error("Failed to begin cmd buffer");
        }
        vkCmdBeginRenderPass(cmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, PipelinePC::instance().getPipeline());
        /*Sets push constats value*/
        vkCmdPushConstants(cmdBuffers[i], PipelinePC::instance().getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4), &MVP[0][0]);
        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(cmdBuffers[i], 0, 1, mesh.getVertexBuffer(), &offsets);
        vkCmdBindIndexBuffer(cmdBuffers[i], mesh.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmdBuffers[i], mesh.getIndicesCount(), 1, 0, 0, 0);
        vkCmdEndRenderPass(cmdBuffers[i]);
        if (vkEndCommandBuffer(cmdBuffers[i]) != VK_SUCCESS)
        {
            LOG.error("Failed to record cmd buffer");
        }
    }
}