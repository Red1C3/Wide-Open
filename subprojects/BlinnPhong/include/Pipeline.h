#pragma once
namespace WideOpenBP{
    class Pipeline{
    private:
        Pipeline();
    public:
        static Pipeline& instance();
        void init();
        void terminate(); //TODO Implement after creating a renderpass
    };
}