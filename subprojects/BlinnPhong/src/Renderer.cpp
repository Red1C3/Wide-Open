#include<Renderer.h>
using namespace WideOpenBP;
using namespace Common;
Renderer::Renderer(){}
Assimp::Importer Renderer::importer;
static inline VkSurfaceFormatKHR getSuitableFormat(uint32_t surfaceFormatsCount,VkSurfaceFormatKHR* formats){
    return formats[0];//TODO LOOK FOR THE BEST FORMAT FIRST
}
void Renderer::init(){
    createInstance();
    createPhysicalDevice();
    createSurface();
    createDevice();
    createSwapchain();
    cmdPool=createCommandPool(device,queueIndex);
    if(cmdPool==nullptr){
        LOG.error("Failed to create command pool");
    }
    LOG.log("Created a command pool successfully");
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
                int index=queueIndex;
                LOG.log(&index,1);
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
    LOG.log("Created a logical device successfully with queues of count");
    LOG.log(&queueCount,1);
    queues=new VkQueue[queueCount];
    for(int i=0;i<queueCount;i++) vkGetDeviceQueue(device,queueIndex,i,&queues[i]);
}
void Renderer::createSwapchain(){
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,surface,&surfaceFormatsCount,nullptr);
    surfaceFormats=new VkSurfaceFormatKHR[surfaceFormatsCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,surface,&surfaceFormatsCount,surfaceFormats);
    swapchainFormat=getSuitableFormat(surfaceFormatsCount,surfaceFormats);
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,surface,&surfaceCapabilities);
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.clipped=VK_FALSE;
    createInfo.compositeAlpha=VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageArrayLayers=1;
    createInfo.imageColorSpace=swapchainFormat.colorSpace;
    createInfo.imageExtent=surfaceCapabilities.currentExtent;
    createInfo.imageFormat=swapchainFormat.format;
    createInfo.imageSharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.imageUsage=VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.minImageCount=surfaceCapabilities.minImageCount;
    createInfo.presentMode=VK_PRESENT_MODE_FIFO_KHR;
    createInfo.surface=surface;
    createInfo.preTransform=VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    if(vkCreateSwapchainKHR(device,&createInfo,ALLOCATOR,&swapchain)!=VK_SUCCESS){
        LOG.error("Couldn't create swap chain");
    }
    LOG.log("Created swapchain successfully");
    uint32_t swapchainImagesCount;
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,nullptr);
    VkImage* swapchainImages=new VkImage[swapchainImagesCount];
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,swapchainImages);
    VkImageViewCreateInfo imgViewCreateInfo{};
    imgViewCreateInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
    imgViewCreateInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
    imgViewCreateInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
    imgViewCreateInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
    imgViewCreateInfo.format=swapchainFormat.format;
    imgViewCreateInfo.image=swapchainImages[0];
    imgViewCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewCreateInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
    imgViewCreateInfo.subresourceRange.baseArrayLayer=0;
    imgViewCreateInfo.subresourceRange.baseMipLevel=0;
    imgViewCreateInfo.subresourceRange.layerCount=1;
    imgViewCreateInfo.subresourceRange.levelCount=1;
    imgViewCreateInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
    if(vkCreateImageView(device,&imgViewCreateInfo,ALLOCATOR,&swapchainImage)!=VK_SUCCESS){
        LOG.error("Failed to create swapchain image view");
    }
    delete[] surfaceFormats;
    delete[] swapchainImages;
}
void Renderer::terminate(){
    vkDestroyCommandPool(device,*cmdPool,ALLOCATOR);
    vkDestroyImageView(device,swapchainImage,ALLOCATOR);
    vkDestroySwapchainKHR(device,swapchain,ALLOCATOR);
    vkDestroyDevice(device,ALLOCATOR);
    vkDestroySurfaceKHR(vkInstance,surface,ALLOCATOR);
    vkDestroyInstance(vkInstance,ALLOCATOR);
    delete cmdPool;
}
Renderer& Renderer::instance(){
    static Renderer renderer;
    return renderer;
}