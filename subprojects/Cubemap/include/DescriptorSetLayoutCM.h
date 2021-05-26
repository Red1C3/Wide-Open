#pragma once
#include<DescriptorSetLayout.h>
namespace WideOpenCM{
    class DescriptorSetLayoutCM:public Common::DescriptorSetLayout{
    private:
        DescriptorSetLayoutCM();
        virtual ~DescriptorSetLayoutCM();
    public:
        static DescriptorSetLayoutCM& instance();
        void init(Common::Renderer* renderer);
    };
}