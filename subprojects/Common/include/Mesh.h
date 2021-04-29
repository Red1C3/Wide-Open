#pragma once
#include<vulkan/vulkan.hpp>
#include<Renderer.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
namespace Common{
    template <class V,class U>
    class Mesh{
    private:
        
        void createIndexBuffer(){
            VkBufferCreateInfo createInfo{};
            createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
            createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;  
            createInfo.usage=VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            createInfo.size=sizeof(uint32_t)*indicesCount;
            if(vkCreateBuffer(renderer->getDevice(),&createInfo,ALLOCATOR,&indexBuffer)!=VK_SUCCESS){
                LOG.error("Failed to create index buffer");
            }
            VkMemoryRequirements memReq;
            vkGetBufferMemoryRequirements(renderer->getDevice(),indexBuffer,&memReq);
            indexBufferMemory=renderer->allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            void* data=(void*)malloc(memReq.size);
            if(vkMapMemory(renderer->getDevice(),indexBufferMemory,0,memReq.size,0,&data)!=VK_SUCCESS){
                LOG.error("Failed to map memory");
            }
            memcpy(data,indices,memReq.size);
            vkUnmapMemory(renderer->getDevice(),indexBufferMemory);
            if(vkBindBufferMemory(renderer->getDevice(),indexBuffer,indexBufferMemory,0)!=VK_SUCCESS){
                LOG.error("Failed to bind memory to buffer");
            }
            LOG.log("Created index buffer successfully");
        }
        void createUniformBuffer(){
            VkBufferCreateInfo createInfo{};
            createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
            createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.usage=VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            createInfo.size=sizeof(uniformBufferObject);
            if(vkCreateBuffer(renderer->getDevice(),&createInfo,ALLOCATOR,&uniformBuffer)!=VK_SUCCESS){
                LOG.error("Failed to create uniform buffer");
            }
            VkMemoryRequirements memReq;
            vkGetBufferMemoryRequirements(renderer->getDevice(),uniformBuffer,&memReq);
            uniformBufferSize=memReq.size;
            uniformBufferMemory=renderer->allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            void* data=(void*)malloc(memReq.size);
            if(vkMapMemory(renderer->getDevice(),uniformBufferMemory,0,memReq.size,0,&data)!=VK_SUCCESS){
                LOG.error("Failed to map memory of uniform buffer");
            }
            memcpy(data,&uniformBufferObject,memReq.size);
            vkUnmapMemory(renderer->getDevice(),uniformBufferMemory);
            if(vkBindBufferMemory(renderer->getDevice(),uniformBuffer,uniformBufferMemory,0)!=VK_SUCCESS){
                LOG.error("Failed to bind memory to uniform buffer");
            }
            LOG.log("Created uniform buffer successfully");
        }
    protected:
        const aiScene* scene;
        Renderer* renderer;
        VkDescriptorSet descriptorSet;
        V* vertices;
        uint32_t* indices;
        uint32_t verticesCount,indicesCount,uniformBufferSize;
        VkBuffer vertexBuffer,indexBuffer,uniformBuffer;
        VkDeviceMemory vertexBufferMemory,indexBufferMemory,uniformBufferMemory;
        U uniformBufferObject;
        virtual V* readVertices(aiMesh* scene)=0;
        virtual void applyUBO()=0;
        void createVertexBuffer(){
            VkBufferCreateInfo createInfo{};
            createInfo.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
            createInfo.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.usage=VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            createInfo.size=sizeof(V)*verticesCount;
            if(vkCreateBuffer(renderer->getDevice(),&createInfo,ALLOCATOR,&vertexBuffer)!=VK_SUCCESS){
                LOG.error("Failed to create vertex buffer");
            }
            VkMemoryRequirements memReq;
            vkGetBufferMemoryRequirements(renderer->getDevice(),vertexBuffer,&memReq);
            vertexBufferMemory=renderer->allocateMemory(memReq,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            void* data=(void*)malloc(memReq.size);
            if(vkMapMemory(renderer->getDevice(),vertexBufferMemory,0,memReq.size,0,&data)!=VK_SUCCESS){
                LOG.error("Failed to map memory");
            }
            memcpy(data,vertices,memReq.size);
            vkUnmapMemory(renderer->getDevice(),vertexBufferMemory);
            if(vkBindBufferMemory(renderer->getDevice(),vertexBuffer,vertexBufferMemory,0)!=VK_SUCCESS){
                LOG.error("Failed to bind memory to buffer");
            }
            LOG.log("Created vertex buffer successfully");
        }
        virtual ~Mesh(){}
    public:
        Mesh(const char* path,U uniformBufferObject,Renderer* renderer){
            this->renderer=renderer;
            this->uniformBufferObject=uniformBufferObject;
            scene=Renderer::importer.ReadFile(path,aiProcess_Triangulate);
            verticesCount=scene->mMeshes[0]->mNumVertices;
            indicesCount=scene->mMeshes[0]->mNumFaces*3;
            indices=new uint32_t[indicesCount];
            for(uint32_t i=0;i<indicesCount;i+=3){
                indices[i]=scene->mMeshes[0]->mFaces[i/3].mIndices[0];
                indices[i+1]=scene->mMeshes[0]->mFaces[i/3].mIndices[1];
                indices[i+2]=scene->mMeshes[0]->mFaces[i/3].mIndices[2];
            }
            LOG.log("Loaded a mesh successfully");
            createIndexBuffer();
            createUniformBuffer();
            renderer->allocateDescriptorSet(&descriptorSet);
        }
        VkDescriptorSet getDescriptorSet(){
            return descriptorSet;
        }
        VkBuffer* getVertexBuffer(){
            return &vertexBuffer;
        }
        VkBuffer getIndexBuffer(){
            return indexBuffer;
        }
        uint32_t getIndicesCount(){
            return indicesCount;
        }
        void cleanup(){
            vkDestroyBuffer(renderer->getDevice(),vertexBuffer,ALLOCATOR);
            vkDestroyBuffer(renderer->getDevice(),indexBuffer,ALLOCATOR);
            vkDestroyBuffer(renderer->getDevice(),uniformBuffer,ALLOCATOR);
            vkFreeMemory(renderer->getDevice(),vertexBufferMemory,ALLOCATOR);
            vkFreeMemory(renderer->getDevice(),indexBufferMemory,ALLOCATOR);
            vkFreeMemory(renderer->getDevice(),uniformBufferMemory,ALLOCATOR);
            delete[] vertices;
            delete[] indices;
        }
    };
}