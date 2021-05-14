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
        glm::vec3 diffuse;
	    float spec;
	    glm::vec3 ambient;
        float dummy;
	    glm::vec3 light;
        float dummier;
	    glm::vec3 view;
    };
    class MeshBPWS:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        static VkSampler sampler;
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
    public:
        MeshBPWS(const char* path,UniformBufferObject uniformBufferObject);
        void applyUBO(VkImageView texture);
        VkDescriptorSet* getSecondDescriptorSet();
        static void destroySharedSampler();
    };
}