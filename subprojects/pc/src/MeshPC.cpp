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