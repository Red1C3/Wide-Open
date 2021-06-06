#pragma once
#include<Mesh.h>
#include<glm/glm.hpp>
#include<RendererPC.h>
namespace WideOpenPC{
    class Vertex{
    public:
        glm::vec3 pos;
    };
    class UniformBufferObject{};
    class MeshPC:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
    public:
        MeshPC(const char* path);
        void cleanup();
    };
}