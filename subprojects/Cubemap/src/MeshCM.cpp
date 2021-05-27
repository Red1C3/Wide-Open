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
#include<MeshCM.h>
using namespace std;
using namespace WideOpenCM;
/*The overriden version does this version work so basically it's not needed,but because the function is pure
virtual, it must be implemented, could have been just an empty body though...*/
void MeshCM::applyUBO(){
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer=uniformBuffer;
    bufferInfo.offset=0;
    bufferInfo.range=VK_WHOLE_SIZE;
    VkWriteDescriptorSet writeInfo{};
    writeInfo.descriptorCount=1;
    writeInfo.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeInfo.dstBinding=0;
    writeInfo.dstSet=descriptorSets[0];
    writeInfo.pBufferInfo=&bufferInfo;
    writeInfo.sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vkUpdateDescriptorSets(renderer->getDevice(),1,&writeInfo,0,nullptr);
}
MeshCM::MeshCM(const char* path,UniformBufferObject ubo):Mesh(path,ubo,&RendererCM::instance()){
    vertices=readVertices(scene->mMeshes[0]);
    createVertexBuffer();
    applyUBO();
    renderer->importer.FreeScene();
    /*The Cube map sampler create info*/
    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType=VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter=VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter=VK_FILTER_LINEAR;
    samplerCreateInfo.mipmapMode=VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU=VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeV=VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeW=VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.anisotropyEnable=VK_FALSE;
    samplerCreateInfo.compareEnable=VK_FALSE;
    samplerCreateInfo.borderColor=VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    if(vkCreateSampler(renderer->getDevice(),&samplerCreateInfo,ALLOCATOR,&sampler)!=VK_SUCCESS){
        LOG.error("Failed to create sampler");
    }
    LOG.log("Created a mesh successfully");
}
Vertex* MeshCM::readVertices(aiMesh* mesh){
    Vertex* vertices=new Vertex[verticesCount];
    /*I only needed vertices position for this example so I only imported them*/
    for(uint32_t i=0;i<verticesCount;++i){
        vertices[i].pos={mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
    }
    return vertices;
}
/*Writes UBO and the cube map (AS SAMPLERCUBE) to the descriptor set*/
void MeshCM::applyUBO(VkImageView texture3d){
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer=uniformBuffer;
    bufferInfo.offset=0;
    bufferInfo.range=VK_WHOLE_SIZE;
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout=VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView=texture3d;
    imageInfo.sampler=sampler;
    VkWriteDescriptorSet writeInfo[2];
    writeInfo[0]=VkWriteDescriptorSet{};
    writeInfo[0].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeInfo[0].descriptorCount=1;
    writeInfo[0].descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeInfo[0].dstBinding=0;
    writeInfo[0].dstSet=descriptorSets[0];
    writeInfo[0].pBufferInfo=&bufferInfo;
    writeInfo[1]=VkWriteDescriptorSet{};
    writeInfo[1].descriptorCount=1;
    writeInfo[1].descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeInfo[1].dstBinding=1;
    writeInfo[1].dstSet=descriptorSets[0];
    writeInfo[1].pImageInfo=&imageInfo;
    writeInfo[1].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vkUpdateDescriptorSets(renderer->getDevice(),2,writeInfo,0,nullptr);
}
void MeshCM::cleanup(){
    Mesh::cleanup();
    vkDestroyImageView(renderer->getDevice(),cubeMapImageView,ALLOCATOR);
    vkDestroyImage(renderer->getDevice(),cubeMapImage,ALLOCATOR);
    vkFreeMemory(renderer->getDevice(),cubeMapMemory,ALLOCATOR);
    vkDestroySampler(renderer->getDevice(),sampler,ALLOCATOR);
}
VkImageView& MeshCM::setupCubeMap(){
    vector<unsigned char> faces[6];
    unsigned int height,width;
    lodepng::decode(faces[0],width,height,"./Assets/Wide-OpenCM/X+.png");
    lodepng::decode(faces[1],width,height,"./Assets/Wide-OpenCM/X-.png");
    lodepng::decode(faces[2],width,height,"./Assets/Wide-OpenCM/Y+.png");
    lodepng::decode(faces[3],width,height,"./Assets/Wide-OpenCM/Y-.png");
    lodepng::decode(faces[4],width,height,"./Assets/Wide-OpenCM/Z+.png");
    lodepng::decode(faces[5],width,height,"./Assets/Wide-OpenCM/Z-.png");
    vector<unsigned char> cubeMapV;
    /*Combines the 6 images into a single RGBA array/vector*/
    cubeMapV.resize(sizeof(unsigned char)*faces[0].size()*6);
    for(uint32_t i=0;i<6;++i){
        memcpy(cubeMapV.data()+i*sizeof(unsigned char)*faces[0].size(),faces[i].data(),sizeof(unsigned char)*faces[i].size());
    }
    uint32_t graphicsQueueFamilyIndex=RendererCM::instance().getGraphicsQueueFamilyIndex();
    /*The image is declared as a 6 2D images array*/
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
    if(vkCreateImage(RendererCM::instance().getDevice(),&imgCreateInfo,ALLOCATOR,&cubeMapImage)!=VK_SUCCESS){
        LOG.error("Failed to create cube map");
    }
    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(RendererCM::instance().getDevice(),cubeMapImage,&memReq);
    cubeMapMemory=RendererCM::instance().allocateMemory(memReq,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if(vkBindImageMemory(RendererCM::instance().getDevice(),cubeMapImage,cubeMapMemory,0)!=VK_SUCCESS){
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
    /*The following are code for preparing the image to hold the actual data
    I changed its layout,copied the staging buffer data,and changed the layout again
    so it is complitible with being used in a descriptor set*/
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
    memBarrier.image=cubeMapImage;
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
    vkCmdCopyBufferToImage(cmdBuffer,stagingBuffer,cubeMapImage,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,6,bics.data());
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
    imgViewCreateInfo.image=cubeMapImage;
    imgViewCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewCreateInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
    imgViewCreateInfo.subresourceRange.baseArrayLayer=0;
    imgViewCreateInfo.subresourceRange.baseMipLevel=0;
    imgViewCreateInfo.subresourceRange.layerCount=6;
    imgViewCreateInfo.subresourceRange.levelCount=1;
    imgViewCreateInfo.viewType=VK_IMAGE_VIEW_TYPE_CUBE;
    if(vkCreateImageView(RendererCM::instance().getDevice(),&imgViewCreateInfo,ALLOCATOR,&cubeMapImageView)!=VK_SUCCESS){
        LOG.error("Failed to create image view");
    }
    vkDestroyBuffer(RendererCM::instance().getDevice(),stagingBuffer,ALLOCATOR);
    vkFreeMemory(RendererCM::instance().getDevice(),bufferMemory,ALLOCATOR);
    return cubeMapImageView;
}