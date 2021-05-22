#pragma once
#include<Mesh.h>
#include<glm/glm.hpp>
#include<RendererCM.h>
namespace WideOpenCM{
    class Vertex{
    public:
        glm::vec3 pos;
    };
    class UniformBufferObject{
    public:
        glm::mat4 MVP;
    };
    class MeshCM:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        VkSampler sampler;
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
    public:
        MeshCM(const char* path,UniformBufferObject ubo);
        void applyUBO(VkImageView texture3d);
        void cleanup();
    };
}