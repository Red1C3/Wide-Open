#include<RendererBPWS.h>
#include<Window.h>
#include<RenderPassBPWSMain.h>
#include<RenderPassBPWSLight.h>
#include<MeshBPWS.h>
#include<LightDSL.h>
#include<MainDSL.h>
#include<glm/gtc/matrix_transform.hpp>
using namespace WideOpenBPWS;
using namespace Common;
using namespace glm;
int main(){
    Window::instance().init(480,640);
    RendererBPWS::instance().init();
    RenderPassBPWSLight::instance().init(&RendererBPWS::instance());
    RenderPassBPWSMain::instance().init(&RendererBPWS::instance());
    LightDSL::instance().init(&RendererBPWS::instance());
    MainDSL::instance().init(&RendererBPWS::instance());
    LightPipeline::instance().init(&RendererBPWS::instance(),&LightDSL::instance(),RenderPassBPWSLight::instance().getRenderPass(),0);
    UniformBufferObject ubo;
    mat4 persp=perspective(45.0f,1.0f,0.1f,100.0f);
    persp[1][1]*=-1;
    ubo.MVP=persp*lookAt(vec3(3,3,3),vec3(0,0,0),vec3(0,0,1));

    MeshBPWS mesh("./Assets/Wide-OpenBP/Cube.gltf",ubo);
    ubo.MVP=ubo.MVP*translate(mat4(1.0f),vec3(0,0,-2));
    MeshBPWS mesh2("./Assets/Wide-OpenBPWS/Plane.gltf",ubo);
    MeshBPWS meshes[2]={mesh,mesh2};
    RenderPassBPWSMain::instance().debugRecord(meshes);

    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType=VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if(vkCreateSemaphore(RendererBPWS::instance().getDevice(),&semaphoreCreateInfo,ALLOCATOR,&acquireSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType=VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags=VK_FENCE_CREATE_SIGNALED_BIT;
    if(vkCreateFence(RendererBPWS::instance().getDevice(),&fenceCreateInfo,ALLOCATOR,&fence)!=VK_SUCCESS){
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if(vkCreateSemaphore(RendererBPWS::instance().getDevice(),&semaphoreCreateInfo,ALLOCATOR,&presentSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue=RendererBPWS::instance().getGraphicsQueue();
    while(!glfwWindowShouldClose(Window::instance().getWindow())){
        glfwPollEvents();
        vkWaitForFences(RendererBPWS::instance().getDevice(),1,&fence,VK_TRUE,UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(RendererBPWS::instance().getDevice(),RendererBPWS::instance().getSwapchain(),0,acquireSemaphore,VK_NULL_HANDLE,&imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount=1;
        VkCommandBuffer cmdBuffer=RenderPassBPWSMain::instance().getCmdBuffer();
        submitInfo.pCommandBuffers=&cmdBuffer;
        submitInfo.waitSemaphoreCount=1;
        submitInfo.pWaitSemaphores=&acquireSemaphore;
        submitInfo.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[]={VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask=waitStage;
        submitInfo.signalSemaphoreCount=1;
        submitInfo.pSignalSemaphores=&presentSemaphore;
        vkResetFences(RendererBPWS::instance().getDevice(),1,&fence);
        if(vkQueueSubmit(graphicsQueue,1,&submitInfo,fence)!=VK_SUCCESS){
            LOG.error("Failed to submit graphics render");
        }
        VkPresentInfoKHR presentInfo{};
        uint32_t a=0;
        presentInfo.pImageIndices=&a;
        presentInfo.sType=VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount=1;
        presentInfo.pWaitSemaphores=&presentSemaphore;
        presentInfo.swapchainCount=1;
        VkSwapchainKHR swapchain=RendererBPWS::instance().getSwapchain();
        presentInfo.pSwapchains=&swapchain;
        if(vkQueuePresentKHR(graphicsQueue,&presentInfo)!=VK_SUCCESS){
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(RendererBPWS::instance().getDevice(),fence,ALLOCATOR);
    vkDestroySemaphore(RendererBPWS::instance().getDevice(),acquireSemaphore,ALLOCATOR);
    vkDestroySemaphore(RendererBPWS::instance().getDevice(),presentSemaphore,ALLOCATOR);
    

    mesh.cleanup();
    mesh2.cleanup();
    LightPipeline::instance().terminate();
    LightDSL::instance().terminate();
    MainDSL::instance().terminate();
    RenderPassBPWSMain::instance().terminate();
    RenderPassBPWSLight::instance().terminate();
    RendererBPWS::instance().terminate();
    Window::instance().terminate();
}