#pragma once
#include<DescriptorSetLayout.h>
namespace WideOpenBPWS{
    class LightDSL:public Common::DescriptorSetLayout{
    private:
        LightDSL();
        virtual ~LightDSL();
    public:
        static LightDSL& instance();
        void init(Common::Renderer* renderer);
    };
}