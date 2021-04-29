#include<MeshBPWS.h>
using namespace WideOpenBPWS;
MeshBPWS::MeshBPWS(const char* path,UniformBufferObject uniformBufferObject):Mesh(path,uniformBufferObject,&RendererBPWS::instance()){
    vertices=readVertices(scene->mMeshes[0]);
    createVertexBuffer();
    applyUBO();
    renderer->importer.FreeScene();
    LOG.log("Created a mesh successfully");
}
Vertex* MeshBPWS::readVertices(aiMesh* mesh){
    Vertex* vertices = new Vertex[verticesCount];
    for(uint32_t i=0;i<verticesCount;++i){
        vertices[i].pos={mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
        vertices[i].normal={mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z};
    }
    return vertices;
}
void MeshBPWS::applyUBO(){
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
    vkUpdateDescriptorSets(renderer->getDevice(),1,&writeInfo,0,nullptr);
}