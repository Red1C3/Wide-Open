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
    /*Note that MVP is the MVP related to the light,the one used in RenderPassBPWSLight*/
    class UniformBufferObject{
    public:
        glm::mat4 MVP;
        glm::mat4 model;
        glm::mat4 lightVP;
        glm::mat4 VP;
        glm::vec3 diffuse;
	    float spec;
	    glm::vec3 ambient;
        float dummy; //for memory padding
	    glm::vec3 light;
        float dummier; //for memory padding
	    glm::vec3 view;
    };
    class MeshBPWS:public Common::Mesh<Vertex,UniformBufferObject>{
    private:
        /*The sampler that will describe how the shadow map will be sampled*/
        static VkSampler sampler;
        Vertex* readVertices(aiMesh* mesh);
        void applyUBO();
    public:
        MeshBPWS(const char* path,UniformBufferObject uniformBufferObject);
        void applyUBO(VkImageView texture);
        /*in this project,each mesh has a descriptor set for each render pass,the first is for
        the light render pass, and the second is for the main render pass*/
        VkDescriptorSet* getSecondDescriptorSet();
        static void destroySharedSampler();
    };
}