#include<RendererBPWS.h>
#include<Window.h>
#include<RenderPassBPWSMain.h>
#include<RenderPassBPWSLight.h>
#include<MeshBPWS.h>
#include<LightDSL.h>
#include<MainDSL.h>
#include<glm/gtc/matrix_transform.hpp>
#include<MainPipeline.h>
#include<RendererCM.h>
#include<RenderPassCM.h>
#include<DescriptorSetLayoutCM.h>
#include<PipelineCM.h>
#include<MeshCM.h>
#include<lodepng.h>
#include<vector>
#include<iostream>
using namespace std;
using namespace WideOpenBPWS;
using namespace WideOpenCM;
using namespace Common;
using namespace glm;
void BPWS();
VkImageView setupCubeMap();
int main(){
    Window::instance().init(480,640);
    RendererCM::instance().init();
    RenderPassCM::instance().init(&RendererCM::instance());
    DescriptorSetLayoutCM::instance().init(&RendererCM::instance());
    PipelineCM::instance().init(&RendererCM::instance(),&DescriptorSetLayoutCM::instance(),RenderPassCM::instance().getRenderPass(),0);
    VkImageView cubeMap=setupCubeMap();
    WideOpenCM::UniformBufferObject ubo;
    mat4 persp=perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
    persp[1][1]*=-1;
    ubo.MVP=persp*lookAt(vec3(0,0,0),vec3(1,1,1),vec3(0,0,1));
    MeshCM mesh("./Assets/Wide-OpenBP/Cube.gltf",ubo);
    mesh.applyUBO(cubeMap);
    RenderPassCM::instance().debugRecord(mesh);
    VkSemaphore acquireSemaphore{};
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType=VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if(vkCreateSemaphore(RendererCM::instance().getDevice(),&semaphoreCreateInfo,ALLOCATOR,&acquireSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkFence fence{};
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType=VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags=VK_FENCE_CREATE_SIGNALED_BIT;
    if(vkCreateFence(RendererCM::instance().getDevice(),&fenceCreateInfo,ALLOCATOR,&fence)!=VK_SUCCESS){
        LOG.error("Failed to create fence");
    }
    VkSemaphore presentSemaphore;
    if(vkCreateSemaphore(RendererCM::instance().getDevice(),&semaphoreCreateInfo,ALLOCATOR,&presentSemaphore)!=VK_SUCCESS){
        LOG.error("Failed to create semaphore");
    }
    VkQueue graphicsQueue=RendererCM::instance().getGraphicsQueue();
    while(!glfwWindowShouldClose(Window::instance().getWindow())){
        glfwPollEvents();
        vkWaitForFences(RendererCM::instance().getDevice(),1,&fence,VK_TRUE,UINT64_MAX);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(RendererCM::instance().getDevice(),RendererCM::instance().getSwapchain(),0,acquireSemaphore,VK_NULL_HANDLE,&imageIndex);
        VkSubmitInfo submitInfo{};
        submitInfo.commandBufferCount=1;
        VkCommandBuffer cmdBuffer=RenderPassCM::instance().getCmdBuffer();
        submitInfo.pCommandBuffers=&cmdBuffer;
        submitInfo.waitSemaphoreCount=1;
        submitInfo.pWaitSemaphores=&acquireSemaphore;
        submitInfo.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStage[]={VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
        submitInfo.pWaitDstStageMask=waitStage;
        submitInfo.signalSemaphoreCount=1;
        submitInfo.pSignalSemaphores=&presentSemaphore;
        vkResetFences(RendererCM::instance().getDevice(),1,&fence);
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
        VkSwapchainKHR swapchain=RendererCM::instance().getSwapchain();
        presentInfo.pSwapchains=&swapchain;
        if(vkQueuePresentKHR(graphicsQueue,&presentInfo)!=VK_SUCCESS){
            LOG.error("Failed to present to swapchain");
        }
    }
    /*Waits for GPU to finish last render*/
    vkQueueWaitIdle(graphicsQueue);
    /*Cleaning up and destroying objects*/
    vkDestroyFence(RendererCM::instance().getDevice(),fence,ALLOCATOR);
    vkDestroySemaphore(RendererCM::instance().getDevice(),acquireSemaphore,ALLOCATOR);
    vkDestroySemaphore(RendererCM::instance().getDevice(),presentSemaphore,ALLOCATOR);
    

    mesh.cleanup();
    PipelineCM::instance().terminate();
    DescriptorSetLayoutCM::instance().terminate();
    RenderPassCM::instance().terminate();
    RendererCM::instance().terminate();
    Window::instance().terminate();
}
void BPWS(){
    RendererBPWS::instance().init();
    RenderPassBPWSLight::instance().init(&RendererBPWS::instance());
    RenderPassBPWSMain::instance().init(&RendererBPWS::instance());
    LightDSL::instance().init(&RendererBPWS::instance());
    MainDSL::instance().init(&RendererBPWS::instance());
    LightPipeline::instance().init(&RendererBPWS::instance(),&LightDSL::instance(),RenderPassBPWSLight::instance().getRenderPass(),0);
    MainPipeline::instance().init(&RendererBPWS::instance(),&MainDSL::instance(),RenderPassBPWSMain::instance().getRenderPass(),0);
    WideOpenBPWS::UniformBufferObject ubo;
    mat4 persp=perspective(45.0f,1.0f,0.1f,100.0f);
    persp[1][1]*=-1;
    ubo.MVP=persp*lookAt(vec3(3,3,3),vec3(0,0,0),vec3(0,0,1));
    ubo.model=mat4(1.0f);
    persp=perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
    persp[1][1]*=-1;
    ubo.lightVP=ubo.MVP;
    ubo.VP=persp*lookAt(vec3(-5,2,3),vec3(0,0,0),vec3(0,0,1));
    ubo.diffuse=vec3(1.0f);
    ubo.ambient=vec3(1.0f);
    ubo.spec=1.0f;
    ubo.view=vec3(-5,2,3);
    ubo.light=vec3(3,3,3);
    MeshBPWS mesh("./Assets/Wide-OpenBP/Cube.gltf",ubo);
    ubo.MVP=ubo.MVP*translate(mat4(1.0f),vec3(0,0,-4));
    ubo.model=translate(mat4(1.0f),vec3(0,0,-4));
    MeshBPWS mesh2("./Assets/Wide-OpenBPWS/Plane.gltf",ubo);
    mesh.applyUBO(RenderPassBPWSLight::instance().getDepthImageView());
    mesh2.applyUBO(RenderPassBPWSLight::instance().getDepthImageView());
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
    MeshBPWS::destroySharedSampler();
    LightPipeline::instance().terminate();
    MainPipeline::instance().terminate();
    LightDSL::instance().terminate();
    MainDSL::instance().terminate();
    RenderPassBPWSMain::instance().terminate();
    RenderPassBPWSLight::instance().terminate();
    RendererBPWS::instance().terminate();
}
VkImageView setupCubeMap(){
    vector<unsigned char> faces[6];
    unsigned int height,width;
    lodepng::decode(faces[0],width,height,"./Assets/Wide-OpenCM/front.png");
    lodepng::decode(faces[1],width,height,"./Assets/Wide-OpenCM/back.png");
    lodepng::decode(faces[2],width,height,"./Assets/Wide-OpenCM/left.png");
    lodepng::decode(faces[3],width,height,"./Assets/Wide-OpenCM/right.png");
    lodepng::decode(faces[4],width,height,"./Assets/Wide-OpenCM/up.png");
    lodepng::decode(faces[5],width,height,"./Assets/Wide-OpenCM/down.png");
    vector<unsigned char> cubeMapV;
    cubeMapV.resize(sizeof(unsigned char)*faces[0].size()*6);
    for(uint32_t i=0;i<6;++i){
        memcpy(cubeMapV.data()+i*sizeof(unsigned char)*faces[0].size(),faces[i].data(),sizeof(unsigned char)*faces[i].size());
    }
    uint32_t graphicsQueueFamilyIndex=RendererCM::instance().getGraphicsQueueFamilyIndex();
    VkImageCreateInfo imgCreateInfo{};
    imgCreateInfo.arrayLayers=6;
    imgCreateInfo.extent={512,512,1};
    imgCreateInfo.flags=VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    imgCreateInfo.format=VK_FORMAT_R8G8B8A8_UNORM;
    imgCreateInfo.imageType=VK_IMAGE_TYPE_2D;
    imgCreateInfo.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    imgCreateInfo.mipLevels=1;
    imgCreateInfo.pQueueFamilyIndices=&graphicsQueueFamilyIndex;
    imgCreateInfo.queueFamilyIndexCount=1;
    imgCreateInfo.samples=VK_SAMPLE_COUNT_1_BIT;
    imgCreateInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    imgCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgCreateInfo.tiling=VK_IMAGE_TILING_OPTIMAL;
    imgCreateInfo.usage=VK_IMAGE_USAGE_SAMPLED_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    VkImage img;
    if(vkCreateImage(RendererCM::instance().getDevice(),&imgCreateInfo,ALLOCATOR,&img)!=VK_SUCCESS){
        LOG.error("Failed to create cube map");
    }
    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(RendererCM::instance().getDevice(),img,&memReq);
    VkDeviceMemory imgMemory=RendererCM::instance().allocateMemory(memReq,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if(vkBindImageMemory(RendererCM::instance().getDevice(),img,imgMemory,0)!=VK_SUCCESS){
        LOG.error("Failed to bind memory to image");
    }
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.queueFamilyIndexCount=1;
    bufferCreateInfo.pQueueFamilyIndices=&graphicsQueueFamilyIndex;
    bufferCreateInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.size=memReq.size;
    bufferCreateInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.usage=VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkBuffer stagingBuffer;
    if(vkCreateBuffer(RendererCM::instance().getDevice(),&bufferCreateInfo,ALLOCATOR,&stagingBuffer)!=VK_SUCCESS){
        LOG.error("failed to create buffer");
    }
    vkGetBufferMemoryRequirements(RendererCM::instance().getDevice(),stagingBuffer,&memReq);
    VkDeviceMemory bufferMemory=RendererCM::instance().allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    if(vkBindBufferMemory(RendererCM::instance().getDevice(),stagingBuffer,bufferMemory,0)!=VK_SUCCESS){
        LOG.error("Failed to bind memory to buffer");
    }
    void* data;
    if(vkMapMemory(RendererCM::instance().getDevice(),bufferMemory,0,VK_WHOLE_SIZE,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map data");
    }
    memcpy(data,cubeMapV.data(),cubeMapV.size()*sizeof(unsigned char));
    vkUnmapMemory(RendererCM::instance().getDevice(),bufferMemory);
    vector<VkBufferImageCopy> bics;
    for(uint32_t i=0;i<6;++i){
        VkBufferImageCopy bic{};
        bic.bufferOffset=i*faces[0].size()*sizeof(unsigned char);
        bic.imageExtent={512,512,1};
        bic.imageSubresource.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
        bic.imageSubresource.baseArrayLayer=i;
        bic.imageSubresource.layerCount=1;
        bic.imageSubresource.mipLevel=0;
        bics.push_back(bic);
    }
    VkImageMemoryBarrier memBarrier{};
    memBarrier.sType=VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memBarrier.oldLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    memBarrier.newLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memBarrier.srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    memBarrier.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    memBarrier.image=img;
    memBarrier.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
    memBarrier.subresourceRange.baseArrayLayer=0;
    memBarrier.subresourceRange.baseMipLevel=0;
    memBarrier.subresourceRange.layerCount=6;
    memBarrier.subresourceRange.levelCount=1;
    memBarrier.dstAccessMask=0;
    memBarrier.srcAccessMask=0;
    VkImageMemoryBarrier memBarrier2=memBarrier;
    memBarrier2.oldLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memBarrier2.newLayout=VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.commandBufferCount=1;
    allocInfo.commandPool=RendererCM::instance().getCmdPool();
    allocInfo.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    VkCommandBuffer cmdBuffer;
    if(vkAllocateCommandBuffers(RendererCM::instance().getDevice(),&allocInfo,&cmdBuffer)!=VK_SUCCESS){
        LOG.error("Failed to allocate cmd buffer");
    }
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if(vkBeginCommandBuffer(cmdBuffer,&beginInfo)!=VK_SUCCESS){
        LOG.error("Failed to begin cmd buffer");
    }
    vkCmdPipelineBarrier(cmdBuffer,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,0,0,nullptr,0,nullptr,1,&memBarrier);
    vkCmdCopyBufferToImage(cmdBuffer,stagingBuffer,img,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,6,bics.data());
    vkCmdPipelineBarrier(cmdBuffer,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,0,0,nullptr,0,nullptr,1,&memBarrier2);
    if(vkEndCommandBuffer(cmdBuffer)!=VK_SUCCESS){
        LOG.error("Failed to end cmd buffer");
    }
    VkSubmitInfo submitInfo{};
    submitInfo.commandBufferCount=1;
    submitInfo.pCommandBuffers=&cmdBuffer;
    submitInfo.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
    if(vkQueueSubmit(RendererCM::instance().getGraphicsQueue(),1,&submitInfo,VK_NULL_HANDLE)!=VK_SUCCESS){
        LOG.error("Failed to submit cmd buffer");
    }
    vkQueueWaitIdle(RendererCM::instance().getGraphicsQueue());
    VkImageViewCreateInfo imgViewCreateInfo{};
    imgViewCreateInfo.components={VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY};
    imgViewCreateInfo.format=VK_FORMAT_R8G8B8A8_UNORM;
    imgViewCreateInfo.image=img;
    imgViewCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewCreateInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
    imgViewCreateInfo.subresourceRange.baseArrayLayer=0;
    imgViewCreateInfo.subresourceRange.baseMipLevel=0;
    imgViewCreateInfo.subresourceRange.layerCount=6;
    imgViewCreateInfo.subresourceRange.levelCount=1;
    imgViewCreateInfo.viewType=VK_IMAGE_VIEW_TYPE_CUBE;
    VkImageView imgView;
    if(vkCreateImageView(RendererCM::instance().getDevice(),&imgViewCreateInfo,ALLOCATOR,&imgView)!=VK_SUCCESS){
        LOG.error("Failed to create image view");
    }
    return imgView;
}