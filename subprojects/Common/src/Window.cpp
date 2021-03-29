#include<Window.h>
using namespace Common;
Window::Window(){}
void Window::init(int height,int width){
    this->height=height;
    this->width=width;
    if(!glfwInit()) Log::instance().error("Failed to init glfw");
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    window=glfwCreateWindow(width,height,"Wide-Open",nullptr,nullptr);
    if (window==nullptr) Log::instance().error("Failed to create Window");
    glfwMakeContextCurrent(window);
    Log::instance().log("Created a window succesfully");
}
GLFWwindow* Window::getWindow(){
    return window;
}
int Window::getHeight(){
    return height;
}
int Window::getWidth(){
    return width;
}
void Window::terminate(){
    glfwDestroyWindow(window);
    glfwTerminate();
    Log::instance().log("Window shutdown successfully");
}
Window& Window::instance(){
    static Window window;
    return window;
}