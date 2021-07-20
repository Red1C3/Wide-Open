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
#include <RendererBPWS.h>
#include <Window.h>
#include <RenderPassBPWSMain.h>
#include <RenderPassBPWSLight.h>
#include <MeshBPWS.h>
#include <LightDSL.h>
#include <MainDSL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <MainPipeline.h>
#include <RendererCM.h>
#include <RenderPassCM.h>
#include <DescriptorSetLayoutCM.h>
#include <PipelineCM.h>
#include <MeshCM.h>
#include <lodepng.h>
#include <RendererPC.h>
#include <RenderPassPC.h>
#include <PipelinePC.h>
#include <MeshPC.h>
#include <vector>
#include <iostream>
using namespace std;
using namespace WideOpenBPWS;
using namespace WideOpenCM;
using namespace WideOpenPC;
using namespace Common;
using namespace glm;
void BPWS();
void cubeMap();
void pipelineCacheAndPushConstants();
int main()
{
    BPWS();
}
void pipelineCacheAndPushConstants()
{
    Window::instance().init(480, 640);
    RendererPC::instance().init();
    RenderPassPC::instance().init(&RendererPC::instance());
    PipelinePC::instance().init(&RendererPC::instance(), RenderPassPC::instance().getRenderPass());
    MeshPC mesh("./Assets/Wide-OpenBP/Cube.gltf");
    RenderPassPC::instance().debugRecord(mesh);

    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(RendererPC::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &acquireSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(RendererPC::instance().getDevice(), &fenceCreateInfo, ALLOCATOR, &fence) != VK_SUCCESS)
    {
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if (vkCreateSemaphore(RendererPC::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &presentSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue = RendererPC::instance().getGraphicsQueue();
    while (!glfwWindowShouldClose(Window::instance().getWindow()))
    {
        glfwPollEvents();
        vkWaitForFences(RendererPC::instance().getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(RendererPC::instance().getDevice(), RendererPC::instance().getSwapchain(), 0, acquireSemaphore, VK_NULL_HANDLE, &imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer cmdBuffer = RenderPassPC::instance().getCmdBuffers()[imageIndex];
        submitInfo.pCommandBuffers = &cmdBuffer;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &acquireSemaphore;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[] = {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask = waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &presentSemaphore;
        vkResetFences(RendererPC::instance().getDevice(), 1, &fence);
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS)
        {
            LOG.error("Failed to submit graphics render");
        }
        VkPresentInfoKHR presentInfo{};
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &presentSemaphore;
        presentInfo.swapchainCount = 1;
        VkSwapchainKHR swapchain = RendererPC::instance().getSwapchain();
        presentInfo.pSwapchains = &swapchain;
        if (vkQueuePresentKHR(graphicsQueue, &presentInfo) != VK_SUCCESS)
        {
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(RendererPC::instance().getDevice(), fence, ALLOCATOR);
    vkDestroySemaphore(RendererPC::instance().getDevice(), acquireSemaphore, ALLOCATOR);
    vkDestroySemaphore(RendererPC::instance().getDevice(), presentSemaphore, ALLOCATOR);

    mesh.cleanup();
    PipelinePC::instance().terminate();
    RenderPassPC::instance().terminate();
    RendererPC::instance().terminate();
    Window::instance().terminate();
}
void cubeMap()
{
    Window::instance().init(480, 640);
    RendererCM::instance().init();
    RenderPassCM::instance().init(&RendererCM::instance());
    DescriptorSetLayoutCM::instance().init(&RendererCM::instance());
    PipelineCM::instance().init(&RendererCM::instance(), &DescriptorSetLayoutCM::instance(), RenderPassCM::instance().getRenderPass(), 0);
    WideOpenCM::UniformBufferObject ubo;
    mat4 persp = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    persp[1][1] *= -1;
    /*change vec3(1,1,1) if you want to change the camera direction*/
    ubo.MVP = persp * lookAt(vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 1));
    MeshCM mesh("./Assets/Wide-OpenBP/Cube.gltf", ubo);
    /*Sets up the cube map resources*/
    VkImageView cubeMap = mesh.setupCubeMap();
    mesh.applyUBO(cubeMap);
    RenderPassCM::instance().debugRecord(mesh);
    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(RendererCM::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &acquireSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(RendererCM::instance().getDevice(), &fenceCreateInfo, ALLOCATOR, &fence) != VK_SUCCESS)
    {
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if (vkCreateSemaphore(RendererCM::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &presentSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue = RendererCM::instance().getGraphicsQueue();
    while (!glfwWindowShouldClose(Window::instance().getWindow()))
    {
        glfwPollEvents();
        vkWaitForFences(RendererCM::instance().getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(RendererCM::instance().getDevice(), RendererCM::instance().getSwapchain(), 0, acquireSemaphore, VK_NULL_HANDLE, &imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer cmdBuffer = RenderPassCM::instance().getCmdBuffers()[imageIndex];
        submitInfo.pCommandBuffers = &cmdBuffer;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &acquireSemaphore;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[] = {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask = waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &presentSemaphore;
        vkResetFences(RendererCM::instance().getDevice(), 1, &fence);
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS)
        {
            LOG.error("Failed to submit graphics render");
        }
        VkPresentInfoKHR presentInfo{};
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &presentSemaphore;
        presentInfo.swapchainCount = 1;
        VkSwapchainKHR swapchain = RendererCM::instance().getSwapchain();
        presentInfo.pSwapchains = &swapchain;
        if (vkQueuePresentKHR(graphicsQueue, &presentInfo) != VK_SUCCESS)
        {
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(RendererCM::instance().getDevice(), fence, ALLOCATOR);
    vkDestroySemaphore(RendererCM::instance().getDevice(), acquireSemaphore, ALLOCATOR);
    vkDestroySemaphore(RendererCM::instance().getDevice(), presentSemaphore, ALLOCATOR);

    mesh.cleanup();
    PipelineCM::instance().terminate();
    DescriptorSetLayoutCM::instance().terminate();
    RenderPassCM::instance().terminate();
    RendererCM::instance().terminate();
    Window::instance().terminate();
}
void BPWS()
{
    Window::instance().init(480, 640);
    RendererBPWS::instance().init();
    RenderPassBPWSLight::instance().init(&RendererBPWS::instance());
    RenderPassBPWSMain::instance().init(&RendererBPWS::instance());
    LightDSL::instance().init(&RendererBPWS::instance());
    MainDSL::instance().init(&RendererBPWS::instance());
    LightPipeline::instance().init(&RendererBPWS::instance(), &LightDSL::instance(), RenderPassBPWSLight::instance().getRenderPass(), 0);
    MainPipeline::instance().init(&RendererBPWS::instance(), &MainDSL::instance(), RenderPassBPWSMain::instance().getRenderPass(), 0);
    WideOpenBPWS::UniformBufferObject ubo;
    mat4 persp = perspective(45.0f, 1.0f, 0.1f, 100.0f);
    persp[1][1] *= -1;
    ubo.MVP = persp * lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 0, 1));
    ubo.model = mat4(1.0f);
    persp = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    persp[1][1] *= -1;
    ubo.lightVP = ubo.MVP;
    ubo.VP = persp * lookAt(vec3(-5, 2, 3), vec3(0, 0, 0), vec3(0, 0, 1));
    ubo.diffuse = vec3(1.0f);
    ubo.ambient = vec3(1.0f);
    ubo.spec = 1.0f;
    ubo.view = vec3(-5, 2, 3);
    ubo.light = vec3(3, 3, 3);
    MeshBPWS mesh("./Assets/Wide-OpenBP/Cube.gltf", ubo);
    ubo.MVP = ubo.MVP * translate(mat4(1.0f), vec3(0, 0, -4));
    ubo.model = translate(mat4(1.0f), vec3(0, 0, -4));
    MeshBPWS mesh2("./Assets/Wide-OpenBPWS/Plane.gltf", ubo);
    mesh.applyUBO(RenderPassBPWSLight::instance().getDepthImageView());
    mesh2.applyUBO(RenderPassBPWSLight::instance().getDepthImageView());
    MeshBPWS meshes[2] = {mesh, mesh2};
    RenderPassBPWSMain::instance().debugRecord(meshes);

    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(RendererBPWS::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &acquireSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(RendererBPWS::instance().getDevice(), &fenceCreateInfo, ALLOCATOR, &fence) != VK_SUCCESS)
    {
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if (vkCreateSemaphore(RendererBPWS::instance().getDevice(), &semaphoreCreateInfo, ALLOCATOR, &presentSemaphore) != VK_SUCCESS)
    {
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue = RendererBPWS::instance().getGraphicsQueue();
    while (!glfwWindowShouldClose(Window::instance().getWindow()))
    {
        glfwPollEvents();
        vkWaitForFences(RendererBPWS::instance().getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(RendererBPWS::instance().getDevice(), RendererBPWS::instance().getSwapchain(), 0, acquireSemaphore, VK_NULL_HANDLE, &imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer cmdBuffer = RenderPassBPWSMain::instance().getCmdBuffers()[imageIndex];
        submitInfo.pCommandBuffers = &cmdBuffer;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &acquireSemaphore;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[] = {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask = waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &presentSemaphore;
        vkResetFences(RendererBPWS::instance().getDevice(), 1, &fence);
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS)
        {
            LOG.error("Failed to submit graphics render");
        }
        VkPresentInfoKHR presentInfo{};
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &presentSemaphore;
        presentInfo.swapchainCount = 1;
        VkSwapchainKHR swapchain = RendererBPWS::instance().getSwapchain();
        presentInfo.pSwapchains = &swapchain;
        if (vkQueuePresentKHR(graphicsQueue, &presentInfo) != VK_SUCCESS)
        {
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(RendererBPWS::instance().getDevice(), fence, ALLOCATOR);
    vkDestroySemaphore(RendererBPWS::instance().getDevice(), acquireSemaphore, ALLOCATOR);
    vkDestroySemaphore(RendererBPWS::instance().getDevice(), presentSemaphore, ALLOCATOR);

    mesh.cleanup();
    mesh2.cleanup();
    MeshBPWS::destroySharedSampler();
    LightPipeline::instance().terminate();
    MainPipeline::instance().terminate();
    LightDSL::instance().terminate();
    MainDSL::instance().terminate();
    RenderPassBPWSMain::instance().terminate();
    RenderPassBPWSLight::instance().terminate();
    RendererBPWS::instance().terminate();
    Window::instance().terminate();
}
