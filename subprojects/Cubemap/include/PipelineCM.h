#pragma once
#include<Pipeline.h>
namespace WideOpenCM{
    class PipelineCM:public Common::Pipeline{
    private:
        PipelineCM();
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
    public:
        static PipelineCM& instance();
    };
}