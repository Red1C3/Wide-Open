#include<MeshPC.h>
using namespace WideOpenPC;
void MeshPC::applyUBO(){}
Vertex* MeshPC::readVertices(aiMesh* mesh){
    Vertex* vertices=new Vertex[verticesCount];
    /*I only needed vertices position for this example so I only imported them*/
    for(uint32_t i=0;i<verticesCount;++i){
        vertices[i].pos={mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
    }
    return vertices;
}
MeshPC::MeshPC(const char* path):Mesh(path,&RendererPC::instance()){
    vertices=readVertices(scene->mMeshes[0]);
    createVertexBuffer();
    renderer->importer.FreeScene();
    LOG.log("Created a mesh successfully");
}
void MeshPC::cleanup(){
    vkDestroyBuffer(renderer->getDevice(),vertexBuffer,ALLOCATOR);
    vkDestroyBuffer(renderer->getDevice(),indexBuffer,ALLOCATOR);
    vkFreeMemory(renderer->getDevice(),vertexBufferMemory,ALLOCATOR);
    vkFreeMemory(renderer->getDevice(),indexBufferMemory,ALLOCATOR);
    delete[] vertices;
    delete[] indices;
}