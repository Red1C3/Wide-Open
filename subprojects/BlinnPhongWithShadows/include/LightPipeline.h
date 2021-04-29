#pragma once
#include<Pipeline.h>
namespace WideOpenBPWS{
    class LightPipeline:public Common::Pipeline{
    private:
        LightPipeline();
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
        static LightPipeline& instance();
    };
}