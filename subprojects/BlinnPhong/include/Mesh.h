#pragma once
#include<glm/glm.hpp>
#include<Renderer.h>
#include<glm/gtc/matrix_transform.hpp>
namespace WideOpenBP{
    class Mesh{
    private:
        struct Vertex{
            glm::vec3 position,normal;
        };
        struct UniformBufferObject{
            glm::mat4 MVP;
            glm::vec3 diffuse;
            float dummy;
            glm::vec3 ambient;
            float dummy2;
            glm::vec3 light;
            float spec;
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
        void applyUBO(UniformBufferObject ubo);
    public:
        Mesh(const char* path);
        void updateUniforms();
        void updateUniforms(glm::mat4 model,glm::vec3 diffuse,glm::vec3 ambient,float spac,glm::vec3 light);
        VkDescriptorSet getDescriptorSet();
        VkBuffer* getVertexBuffer();
        VkBuffer getIndexBuffer();
        uint32_t getIndicesCount();
        void cleanup();
    };
}