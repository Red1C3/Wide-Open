#pragma once
#include<Pipeline.h>
namespace WideOpenBPWS{
    class MainPipeline:public Common::Pipeline{
    private:
        MainPipeline();
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
        static MainPipeline& instance();
    };
}