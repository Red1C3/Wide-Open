#include<Mesh.h>
using namespace WideOpenBP;
using namespace Assimp;
using namespace glm;
Mesh::Mesh(const char* path){
    const aiScene* scene=Renderer::importer.ReadFile(path,aiProcess_Triangulate);
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
    Renderer::importer.FreeScene();
    LOG.log("Loaded a mesh successfully");
    createVertexBuffer();
    createIndexBuffer();
}
void Mesh::createVertexBuffer(){
    VkBufferCreateInfo createInfo{};
    createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.usage=VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    createInfo.size=sizeof(Vertex)*verticesCount;
    if(vkCreateBuffer(RENDERER.getDevice(),&createInfo,ALLOCATOR,&vertexBuffer)!=VK_SUCCESS){
        LOG.error("Failed to create vertex buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(DEVICE,vertexBuffer,&memReq);
    vertexBufferMem=RENDERER.allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data=(void*)malloc(memReq.size);
    if(vkMapMemory(DEVICE,vertexBufferMem,0,memReq.size,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map memory");
    }
    memcpy(data,vertices,memReq.size);
    vkUnmapMemory(DEVICE,vertexBufferMem);
    if(vkBindBufferMemory(DEVICE,vertexBuffer,vertexBufferMem,0)!=VK_SUCCESS){
        LOG.error("Failed to bind buffer to memory");
    }
    LOG.log("Created vertex buffer successfully");
}
void Mesh::createIndexBuffer(){
    VkBufferCreateInfo createInfo{};
    createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.usage=VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    createInfo.size=sizeof(unsigned int)*indicesCount;
    if(vkCreateBuffer(DEVICE,&createInfo,ALLOCATOR,&indexBuffer)){
        LOG.error("Failed to create index buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(DEVICE,indexBuffer,&memReq);
    indexBufferMem=RENDERER.allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    void* data=(void*)malloc(memReq.size);
    if(vkMapMemory(DEVICE,indexBufferMem,0,memReq.size,0,&data)!=VK_SUCCESS){
        LOG.error("Failed to map memory");
    }
    memcpy(data,indices,memReq.size);
    vkUnmapMemory(DEVICE,indexBufferMem);
    if(vkBindBufferMemory(DEVICE,indexBuffer,indexBufferMem,0)!=VK_SUCCESS){
        LOG.error("Failed to bind buffer to memory");
    }
    LOG.log("Created index buffer successfully");
}
void Mesh::cleanup(){
    vkDestroyBuffer(DEVICE,indexBuffer,ALLOCATOR);
    vkDestroyBuffer(DEVICE,vertexBuffer,ALLOCATOR);
    vkFreeMemory(DEVICE,vertexBufferMem,ALLOCATOR);
    vkFreeMemory(DEVICE,indexBufferMem,ALLOCATOR);
    delete[] vertices;
    delete[] indices;
}