#pragma once
#include<Mesh.h>
#include<glm/glm.hpp>
#include<RendererBPWS.h>
#include<MainDSL.h>
namespace WideOpenBPWS{
    class Vertex{
    public:
        glm::vec3 pos,normal;
    };
    class UniformBufferObject{
    public:
        glm::mat4 MVP;
        glm::mat4 model;
        glm::mat4 lightVP;
        glm::mat4 VP;
    };
    class MeshBPWS:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
        VkDescriptorSet secondDescriptorSet;
    public:
        MeshBPWS(const char* path,UniformBufferObject uniformBufferObject);
        void applyUBO(VkImageView texture);
        VkDescriptorSet* getSecondDescriptorSet();
    };
}