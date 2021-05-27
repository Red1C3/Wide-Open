/*MIT License

Copyright (c) 2021 Mohammad Issawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
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