#pragma once
#include<glm/glm.hpp>
#include<RendererBP.h>
#include<glm/gtc/matrix_transform.hpp>
namespace WideOpenBP{
    class MeshBP{
    private:
        struct Vertex{
            glm::vec3 position,normal;
        };
        struct UniformBufferObject{
            glm::mat4 VP;
            glm::mat4 model;
            glm::vec3 diffuse;
            float spec;
            glm::vec3 ambient;
            float dummy;
            glm::vec3 light;
            float fummy;
            glm::vec3 view;
        };
        UniformBufferObject ubo;
        Vertex* vertices;
        uint32_t verticesCount,indicesCount;
        unsigned int* indices;
        VkBuffer vertexBuffer,indexBuffer,uniformBuffer;
        VkDeviceMemory vertexBufferMem,indexBufferMem,uniformBufferMem;
        uint32_t uniformBufferSize;
        VkDescriptorSet descriptorSet;
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffer();
        /*Applies Uniform Buffer to Descriptor Set*/
        void applyUBO(UniformBufferObject ubo);
        /*Only updates the Uniform Buffer*/
        void updateUBO(UniformBufferObject ubo);
    public:
        MeshBP(const char* path);
        void updateUniforms();//Depracated
        void updateUniforms(glm::mat4 model,glm::vec3 diffuse,glm::vec3 ambient,float spac,glm::vec3 light,bool update);
        VkDescriptorSet getDescriptorSet();
        VkBuffer* getVertexBuffer();
        VkBuffer getIndexBuffer();
        uint32_t getIndicesCount();
        void cleanup();
    };
}