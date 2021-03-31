#include<Renderer.h>
using namespace WideOpenBP;
Renderer::Renderer(){}
void Renderer::init(){
    createInstance();
    createPhysicalDevice();
    createSurface();
    createDevice();
}
void Renderer::createInstance(){
    VkInstanceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    glfwExtensions=glfwGetRequiredInstanceExtensions(&ExtCount);
    createInfo.enabledLayerCount=layersCount;
    createInfo.ppEnabledLayerNames=&validationLayer;
    createInfo.enabledExtensionCount=ExtCount;
    createInfo.ppEnabledExtensionNames=glfwExtensions;
    if(vkCreateInstance(&createInfo,ALLOCATOR,&vkInstance)!=VK_SUCCESS){
        LOG.error("Failed to create vulkan instance");
    }
    LOG.log("Created vulkan instance successfully");
}
void Renderer::createPhysicalDevice(){
    uint32_t physicalDevicesCount;
    vkEnumeratePhysicalDevices(vkInstance,&physicalDevicesCount,nullptr);
    VkPhysicalDevice* physicalDevices=new VkPhysicalDevice[physicalDevicesCount];
    vkEnumeratePhysicalDevices(vkInstance,&physicalDevicesCount,physicalDevices);
    for(uint32_t i=0;i<physicalDevicesCount;i++){
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i],&properties);
        if(properties.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            physicalDevice=physicalDevices[i];
            break;
        }
    }
    delete[] physicalDevices;
    LOG.log("Used discrete GPU as physical device successfully");
}
void Renderer::createSurface(){
    if(glfwCreateWindowSurface(vkInstance,WINDOW.getWindow(),ALLOCATOR,&surface)!=VK_SUCCESS)
        LOG.error("Failed to get surface");
}
void Renderer::createDevice(){
    VkDeviceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.enabledLayerCount=layersCount;
    createInfo.enabledExtensionCount=1;
    const char* extensions[]={VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.ppEnabledExtensionNames=extensions; //NOTE : May should be GLFW :/
    createInfo.ppEnabledLayerNames=&validationLayer;
    uint32_t queuesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queuesCount,nullptr);
    VkQueueFamilyProperties* queuesFamilies=new VkQueueFamilyProperties[queuesCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queuesCount,queuesFamilies);
    for(uint32_t i=0;i<queuesCount;i++){
        if(queuesFamilies[i].queueFlags&VK_QUEUE_GRAPHICS_BIT){
            VkBool32 supportsPresent=VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,i,surface,&supportsPresent);
            if(supportsPresent==VK_TRUE){
                queueCount=queuesFamilies[i].queueCount;
                queueIndex=i;
                LOG.log("Found a graphics and presentation queue at index");
                LOG.log(&queueIndex,1);
                break;
            }
        }
    }
    delete[] queuesFamilies;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueCount=queueCount;
    queueCreateInfo.queueFamilyIndex=queueIndex;
    float* priorities=new float[queueCount];
    for(int i=0;i<queueCount;i++) priorities[i]=1.0f;
    queueCreateInfo.pQueuePriorities=priorities; 
    createInfo.pQueueCreateInfos=&queueCreateInfo;
    createInfo.queueCreateInfoCount=1;
    VkResult err;
    err=vkCreateDevice(physicalDevice,&createInfo,ALLOCATOR,&device);
    if(err!=VK_SUCCESS){
        int result=err;
        LOG.log(&result,1);
        LOG.error("Failed to create a logical device");
    }
    delete[] priorities;
    LOG.log("Created a logical device successfully");
}
void Renderer::createSwapchain(){
    
}
void Renderer::terminate(){
    vkDestroyDevice(device,ALLOCATOR);
    vkDestroySurfaceKHR(vkInstance,surface,ALLOCATOR);
    vkDestroyInstance(vkInstance,ALLOCATOR);
}
Renderer& Renderer::instance(){
    static Renderer renderer;
    return renderer;
}