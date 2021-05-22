#include<MeshCM.h>
using namespace WideOpenCM;
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
    for(uint32_t i=0;i<verticesCount;++i){
        vertices[i].pos={mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
    }
    return vertices;
}
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
    vkDestroySampler(renderer->getDevice(),sampler,ALLOCATOR);
}