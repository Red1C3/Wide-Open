#pragma once
#include<Mesh.h>
#include<glm/glm.hpp>
#include<RendererBPWS.h>
namespace WideOpenBPWS{
    class Vertex{
    public:
        glm::vec3 pos,normal;
    };
    class UniformBufferObject{
    public:
        glm::mat4 MVP;
    };
    class MeshBPWS:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
    public:
        MeshBPWS(const char* path,UniformBufferObject uniformBufferObject);
    };
}