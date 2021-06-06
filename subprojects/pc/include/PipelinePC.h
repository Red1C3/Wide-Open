#pragma once
#include<Pipeline.h>
#include<fstream>
#include<vector>
namespace WideOpenPC{
    class PipelinePC:public Common::Pipeline{
    private:
        PipelinePC();
        void createShaderModules();
        void createLayout();
        void createShaderStages();
        void createVertexInputState();
        void createInputAssemblyState();
        void createViewportState();
        void createRasterState();
        void createMultisampleState();
        void createDepthStencilState();
        void createColorBlendState();
        void createDynamicState();
        std::fstream cacheFile;
        std::vector<char> cacheData;
        VkPipelineCache pipelineCache;
    public:
        static PipelinePC& instance();
        void init(Common::Renderer* renderer,VkRenderPass renderPass);
        void terminate();
    };
}