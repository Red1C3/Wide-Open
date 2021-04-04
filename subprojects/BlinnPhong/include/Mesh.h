#pragma once
#include<glm/glm.hpp>
#include<Renderer.h>
namespace WideOpenBP{
    class Mesh{
    private:
        struct Vertex{
            glm::vec3 position,normal;
        };
        Vertex* vertices;
        uint32_t verticesCount,indicesCount;
        unsigned int* indices;
        VkBuffer vertexBuffer,indexBuffer,uniformBuffer;
        VkDeviceMemory vertexBufferMem,indexBufferMem,uniformBufferMem;
        VkDescriptorSet descriptorSet;
        void createVertexBuffer();
        void createIndexBuffer();
    public:
        Mesh(const char* path);
        void cleanup();
    };
}