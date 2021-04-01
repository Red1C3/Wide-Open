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
}