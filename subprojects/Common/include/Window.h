#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<Log.h>
namespace Common{
    class Window{
    private:
        Window();
        GLFWwindow* window;
        int height,width;
    public:
        void init(int height,int width);
        GLFWwindow* getWindow();
        int getHeight();
        int getWidth();
        void terminate();
        static Window& instance();
    };
}