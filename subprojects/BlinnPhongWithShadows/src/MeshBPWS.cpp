#include<MeshBPWS.h>
using namespace WideOpenBPWS;
MeshBPWS::MeshBPWS(const char* path,UniformBufferObject uniformBufferObject):Mesh(path,uniformBufferObject,&RendererBPWS::instance()){
    vertices=readVertices(scene->mMeshes[0]);
    createVertexBuffer();
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