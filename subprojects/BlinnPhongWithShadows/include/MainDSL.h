#pragma once
#include<DescriptorSetLayout.h>
namespace WideOpenBPWS{
    class MainDSL:public Common::DescriptorSetLayout{
    private:
        MainDSL();
        virtual ~MainDSL();
    public:
        static MainDSL& instance();
        void init(Common::Renderer* renderer);
    };
}