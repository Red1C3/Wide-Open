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
#include<MeshBP.h>
using namespace WideOpenBP;
using namespace Assimp;
using namespace glm;
MeshBP::MeshBP(const char* path){
    const aiScene* scene=RendererBP::importer.ReadFile(path,aiProcess_Triangulate);
    verticesCount=scene->mMeshes[0]->mNumVertices;
    vertices=new Vertex[verticesCount];
    for(uint32_t i=0;i<verticesCount;i++){
        vertices[i].position={scene->mMeshes[0]->mVertices[i].x,scene->mMeshes[0]->mVertices[i].y,scene->mMeshes[0]->mVertices[i].z};
        vertices[i].normal={scene->mMeshes[0]->mNormals[i].x,scene->mMeshes[0]->mNormals[i].y,scene->mMeshes[0]->mNormals[i].z};
    }
    indicesCount=scene->mMeshes[0]->mNumFaces*3;
    indices=new unsigned int[indicesCount];
    for(uint32_t i=0;i<indicesCount;i+=3){
        indices[i]=scene->mMeshes[0]->mFaces[i/3].mIndices[0];
        indices[i+1]=scene->mMeshes[0]->mFaces[i/3].mIndices[1];
        indices[i+2]=scene->mMeshes[0]->mFaces[i/3].mIndices[2];
    }
    RendererBP::importer.FreeScene();
    LOG.log("Loaded a mesh successfully");
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffer();
    RENDERERBP.allocateDescriptorSet(&descriptorSet);
}
void MeshBP::createVertexBuffer(){
    VkBufferCreateInfo createInfo{};
    createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.usage=VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    createInfo.size=sizeof(Vertex)*verticesCount;
    if(vkCreateBuffer(RENDERERBP.getDevice(),&createInfo,ALLOCATOR,&vertexBuffer)!=VK_SUCCESS){
        LOG.error("Failed to create vertex buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(DEVICEBP,vertexBuffer,&memReq);
    vertexBufferMem=RENDERERBP.allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    if(vkMapMemory(DEVICEBP,vertexBufferMem,0,memReq.size,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map memory");
    }
    memcpy(data,vertices,memReq.size);
    vkUnmapMemory(DEVICEBP,vertexBufferMem);
    if(vkBindBufferMemory(DEVICEBP,vertexBuffer,vertexBufferMem,0)!=VK_SUCCESS){
        LOG.error("Failed to bind buffer to memory");
    }
    LOG.log("Created vertex buffer successfully");
}
void MeshBP::createIndexBuffer(){
    VkBufferCreateInfo createInfo{};
    createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.usage=VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    createInfo.size=sizeof(unsigned int)*indicesCount;
    if(vkCreateBuffer(DEVICEBP,&createInfo,ALLOCATOR,&indexBuffer)){
        LOG.error("Failed to create index buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(DEVICEBP,indexBuffer,&memReq);
    indexBufferMem=RENDERERBP.allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    void* data;
    if(vkMapMemory(DEVICEBP,indexBufferMem,0,memReq.size,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map memory");
    }
    memcpy(data,indices,memReq.size);
    vkUnmapMemory(DEVICEBP,indexBufferMem);
    if(vkBindBufferMemory(DEVICEBP,indexBuffer,indexBufferMem,0)!=VK_SUCCESS){
        LOG.error("Failed to bind buffer to memory");
    }
    LOG.log("Created index buffer successfully");
}
void MeshBP::createUniformBuffer(){
    VkBufferCreateInfo createInfo{};
    createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.usage=VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    createInfo.size=sizeof(UniformBufferObject);
    if(vkCreateBuffer(DEVICEBP,&createInfo,ALLOCATOR,&uniformBuffer)!=VK_SUCCESS){
        LOG.error("Failed to create uniform buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(DEVICEBP,uniformBuffer,&memReq);
    uniformBufferSize=memReq.size;
    uniformBufferMem=RENDERERBP.allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    if(vkBindBufferMemory(DEVICEBP,uniformBuffer,uniformBufferMem,0)!=VK_SUCCESS){
        LOG.error("Failed to bind memory to buffer");
    }
    LOG.log("Created uniform buffer successfully");
}
VkDescriptorSet MeshBP::getDescriptorSet(){
    return descriptorSet;
}
VkBuffer MeshBP::getIndexBuffer(){
    return indexBuffer;
}
VkBuffer* MeshBP::getVertexBuffer(){
    return &vertexBuffer;
}
void MeshBP::updateUniforms(){
    UniformBufferObject ubo;
    mat4 perp=perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
    perp[1][1]*=-1;
    ubo.VP=perp*lookAt(vec3{3,3,3},vec3{0,0,0},vec3{0,0,1});
    applyUBO(ubo);
}
void MeshBP::updateUniforms(mat4 model,vec3 diffuse,vec3 ambient,float spec,vec3 light,bool update){
    UniformBufferObject ubo;
    mat4 perp=perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
    perp[1][1]*=-1;
    /*Camera's position, feel free to change it*/
    vec3 viewVec=vec3(3,3,3);
    ubo.VP=perp*lookAt(viewVec,vec3{0,0,0},vec3{0,0,1});
    ubo.model=model;
    ubo.diffuse=diffuse;
    ubo.ambient=ambient;
    ubo.spec=spec;
    ubo.light=light;
    ubo.view=viewVec;
    if(!update)
        applyUBO(ubo);
    else 
        updateUBO(ubo);
}
void MeshBP::updateUBO(UniformBufferObject ubo){
    void* data;
    if(vkMapMemory(DEVICEBP,uniformBufferMem,0,sizeof(ubo),0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map memory to update uniform");
    }
    memcpy(data,&ubo,sizeof(ubo));
    vkUnmapMemory(DEVICEBP,uniformBufferMem);
}
void MeshBP::applyUBO(UniformBufferObject ubo){
    void* data;
    if(vkMapMemory(DEVICEBP,uniformBufferMem,0,uniformBufferSize,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map uniform buffer memory");
    }
    memcpy(data,&ubo,sizeof(UniformBufferObject));
    vkUnmapMemory(DEVICEBP,uniformBufferMem);
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer=uniformBuffer;
    bufferInfo.offset=0;
    bufferInfo.range=VK_WHOLE_SIZE;
    VkWriteDescriptorSet writeInfo{};
    writeInfo.sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeInfo.descriptorCount=1;
    writeInfo.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeInfo.dstBinding=0;
    writeInfo.dstSet=descriptorSet;
    writeInfo.pBufferInfo=&bufferInfo;
    vkUpdateDescriptorSets(DEVICEBP,1,&writeInfo,0,nullptr);
}
uint32_t MeshBP::getIndicesCount(){
    return indicesCount;
}
void MeshBP::cleanup(){
    vkDestroyBuffer(DEVICEBP,indexBuffer,ALLOCATOR);
    vkDestroyBuffer(DEVICEBP,vertexBuffer,ALLOCATOR);
    vkDestroyBuffer(DEVICEBP,uniformBuffer,ALLOCATOR);
    vkFreeMemory(DEVICEBP,uniformBufferMem,ALLOCATOR);
    vkFreeMemory(DEVICEBP,vertexBufferMem,ALLOCATOR);
    vkFreeMemory(DEVICEBP,indexBufferMem,ALLOCATOR);
    delete[] vertices;
    delete[] indices;
}