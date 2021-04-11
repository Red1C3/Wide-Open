#include<Definations.i>
#include<Log.h>
#include<Window.h>
#include<Renderer.h>
#include<Mesh.h>
#include<Renderpass.h>
#include<iostream>
#include<Pipeline.h>
#include<DescriptorSetLayout.h>
using namespace std;
using namespace WideOpenBP;
using namespace glm;
void renderBlinnPhong(){
    /*Setting up essential components*/
    WINDOW.init(480,640);
    RENDERER.init();
    RENDERPASS.init();
    LAYOUT.init();
    PIPELINE.init();
    /*Loading mesh and setting up uniforms*/
    Mesh mesh("./Assets/Wide-OpenBP/Cube.gltf");
    vec3 light=vec3(2.5f,3,3);
    mesh.updateUniforms(mat4(1.0f),vec3(1.0f),vec3(1.0f),2.0f,light,false);
    /*Recording RenderPass*/
    RENDERPASS.record(PIPELINE.getPipeline(),PIPELINE.getPipelineLayout(),mesh);
    /*Creating synchronous objects*/
    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType=VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if(vkCreateSemaphore(DEVICE,&semaphoreCreateInfo,ALLOCATOR,&acquireSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType=VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags=VK_FENCE_CREATE_SIGNALED_BIT;
    if(vkCreateFence(DEVICE,&fenceCreateInfo,ALLOCATOR,&fence)!=VK_SUCCESS){
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if(vkCreateSemaphore(DEVICE,&semaphoreCreateInfo,ALLOCATOR,&presentSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue;
    vkGetDeviceQueue(DEVICE,RENDERER.getGraphicsQueueIndex(),0,&graphicsQueue);
    while(!glfwWindowShouldClose(WINDOW.getWindow())){
        glfwPollEvents();
        vkWaitForFences(DEVICE,1,&fence,VK_TRUE,UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(DEVICE,RENDERER.getSwapchain(),0,acquireSemaphore,VK_NULL_HANDLE,&imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount=1;
        VkCommandBuffer cmdBuffer=RENDERPASS.getFramebuffers()[imageIndex].cmdBuffer;
        submitInfo.pCommandBuffers=&cmdBuffer;
        submitInfo.waitSemaphoreCount=1;
        submitInfo.pWaitSemaphores=&acquireSemaphore;
        submitInfo.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[]={VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask=waitStage;
        submitInfo.signalSemaphoreCount=1;
        submitInfo.pSignalSemaphores=&presentSemaphore;
        vkResetFences(DEVICE,1,&fence);
        if(vkQueueSubmit(graphicsQueue,1,&submitInfo,fence)!=VK_SUCCESS){
            LOG.error("Failed to submit graphics render");
        }
        VkPresentInfoKHR presentInfo{};
        presentInfo.pImageIndices=&imageIndex;
        presentInfo.sType=VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount=1;
        presentInfo.pWaitSemaphores=&presentSemaphore;
        presentInfo.swapchainCount=1;
        VkSwapchainKHR swapchain=RENDERER.getSwapchain();
        presentInfo.pSwapchains=&swapchain;
        if(vkQueuePresentKHR(graphicsQueue,&presentInfo)!=VK_SUCCESS){
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(DEVICE,fence,ALLOCATOR);
    vkDestroySemaphore(DEVICE,acquireSemaphore,ALLOCATOR);
    vkDestroySemaphore(DEVICE,presentSemaphore,ALLOCATOR);
    mesh.cleanup();
    PIPELINE.terminate();
    LAYOUT.terminate();
    RENDERPASS.terminate(); 
    RENDERER.terminate();
    WINDOW.terminate();
}
int main(){
    renderBlinnPhong();
}