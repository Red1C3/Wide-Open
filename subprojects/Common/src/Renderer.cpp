#include<Renderer.h>
using namespace Common;
Renderer::Renderer(){}
Assimp::Importer Renderer::importer;
void Renderer::init(){
    createInstance();
    createPhysicalDevice();
    createSurface();
    createDevice();
    createSwapchain();
    cmdPool=createCommandPool(device,graphicsQueueFamily.index);
    if(cmdPool==nullptr)
        LOG.error("Failed to create command pool");
    createDescriptorPool();
}
void Renderer::createInstance(){
    VkApplicationInfo appInfo{};
    appInfo.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext=nullptr;
    appInfo.apiVersion=VK_API_VERSION_1_2;
    appInfo.applicationVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.engineVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.pApplicationName="Wide-Open";
    appInfo.pEngineName="Wide-Open";
    VkInstanceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext=nullptr;
    createInfo.pApplicationInfo=&appInfo;
    glfwExtensions=glfwGetRequiredInstanceExtensions(&extensionsCount);
    createInfo.enabledExtensionCount=extensionsCount;
    createInfo.enabledLayerCount=layersCount;
    createInfo.ppEnabledExtensionNames=glfwExtensions;
    createInfo.ppEnabledLayerNames=&validationLayer;
    if(vkCreateInstance(&createInfo,ALLOCATOR,&vulkanInstance)!=VK_SUCCESS){
        LOG.error("Failed to create vulkan instance");
    }
    LOG.log("Created vulkan instance successfully");
}
void Renderer::createPhysicalDevice(){
    physicalDevice=Common::getPhysicalDevice(vulkanInstance);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice,&memoryProperties);
}
void Renderer::createSurface(){
    if(glfwCreateWindowSurface(vulkanInstance,Window::instance().getWindow(),ALLOCATOR,&surface)!=VK_SUCCESS){
        LOG.error("Failed to get surface");
    }
}
void Renderer::createDevice(){
    VkDeviceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.enabledExtensionCount=1;
    createInfo.enabledLayerCount=layersCount;
    const char* extensions[]={VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.ppEnabledExtensionNames=extensions;
    createInfo.ppEnabledLayerNames=&validationLayer;
    uint32_t queueFamiliesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamiliesCount,nullptr);
    VkQueueFamilyProperties* queueFamilies=new VkQueueFamilyProperties[queueFamiliesCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamiliesCount,queueFamilies);
    bool graphicsQueueFound=false,presentQueueFound=false;
    for(uint32_t i=0;i<queueFamiliesCount;++i){
        if(graphicsQueueFound&&presentQueueFound) break;
        if(!graphicsQueueFound){
            if(queueFamilies[i].queueFlags&VK_QUEUE_GRAPHICS_BIT){
                graphicsQueueFound=true;
                graphicsQueueFamily.count=queueFamilies[i].queueCount;
                graphicsQueueFamily.index=i;
            }
        }
        if(!presentQueueFound){
            VkBool32 supportsPresent=VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,i,surface,&supportsPresent);
            if(supportsPresent==VK_TRUE){
                presentQueueFound=true;
                presentQueueFamily.count=queueFamilies[i].queueCount;
                presentQueueFamily.index=i;
            }
        }
    }
    delete[] queueFamilies;
    VkDeviceQueueCreateInfo* queuesCreateInfos;
    float* priorities;
    uint32_t queuesCount;
    if(graphicsQueueFamily.index==presentQueueFamily.index){
        queuesCount=1;
        queuesCreateInfos=new VkDeviceQueueCreateInfo[1];
        queuesCreateInfos[0]=VkDeviceQueueCreateInfo{};
        queuesCreateInfos[0].sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queuesCreateInfos[0].queueCount=graphicsQueueFamily.count;
        queuesCreateInfos[0].queueFamilyIndex=graphicsQueueFamily.index;
        priorities=new float[graphicsQueueFamily.count];
        for(uint32_t i=0;i<graphicsQueueFamily.count;++i) priorities[i]=1.0f;
        queuesCreateInfos[0].pQueuePriorities=priorities;
    }else{
        //TODO
    }
    createInfo.queueCreateInfoCount=queuesCount;
    createInfo.pQueueCreateInfos=queuesCreateInfos;
    if(vkCreateDevice(physicalDevice,&createInfo,ALLOCATOR,&device)!=VK_SUCCESS){
        LOG.error("Failed to create logical device");
    }
    delete[] priorities;
    delete[] queuesCreateInfos;
    LOG.log("Created a logical device successfully");
    if(graphicsQueueFamily.index==presentQueueFamily.index){
        graphicsQueues=new VkQueue[graphicsQueueFamily.count];
        presentQueues=graphicsQueues;
        for(uint32_t i=0;i<graphicsQueueFamily.count;++i) vkGetDeviceQueue(device,graphicsQueueFamily.index,i,&graphicsQueues[i]);
    }else{
        //TODO
    }
}
void Renderer::createSwapchain(){
    uint32_t surfaceFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,surface,&surfaceFormatsCount,nullptr);
    VkSurfaceFormatKHR* surfaceFormats=new VkSurfaceFormatKHR[surfaceFormatsCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,surface,&surfaceFormatsCount,surfaceFormats);
    swapchainFormat=surfaceFormats[0];//TODO find best format first and use it
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,surface,&surfaceCapabilities);
    swapchainExtent=surfaceCapabilities.currentExtent;
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.clipped=VK_FALSE;
    createInfo.compositeAlpha=VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageArrayLayers=1;
    createInfo.imageColorSpace=swapchainFormat.colorSpace;
    createInfo.imageExtent=swapchainExtent;
    createInfo.imageFormat=swapchainFormat.format;
    createInfo.imageSharingMode=VK_SHARING_MODE_EXCLUSIVE;
    createInfo.imageUsage=VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.minImageCount=surfaceCapabilities.minImageCount;
    createInfo.presentMode=VK_PRESENT_MODE_IMMEDIATE_KHR;
    createInfo.surface=surface;
    createInfo.preTransform=VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    if(vkCreateSwapchainKHR(device,&createInfo,ALLOCATOR,&swapchain)!=VK_SUCCESS){
        LOG.error("Failed to create swapchain");
    }
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,nullptr);
    VkImage* swapchainImages=new VkImage[swapchainImagesCount];
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,swapchainImages);
    swapchainImageViews=new VkImageView[swapchainImagesCount];
    for(uint32_t i=0;i<swapchainImagesCount;++i){
        VkImageViewCreateInfo viewInfo{};
        viewInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.format=swapchainFormat.format;
        viewInfo.image=swapchainImages[i];
        viewInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseArrayLayer=0;
        viewInfo.subresourceRange.baseMipLevel=0;
        viewInfo.subresourceRange.layerCount=1;
        viewInfo.subresourceRange.levelCount=1;
        viewInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
        if(vkCreateImageView(device,&viewInfo,ALLOCATOR,&swapchainImageViews[i])!=VK_SUCCESS){
            LOG.error("Failed to create a swapchain image view");
        }
    }
    delete[] surfaceFormats;
    delete[] swapchainImages;
}
VkDeviceMemory Renderer::allocateMemory(VkMemoryRequirements memReq,VkMemoryPropertyFlags properties){
    uint32_t memoryIndex;
    for(uint32_t i=0;i<memoryProperties.memoryTypeCount;++i){
        const uint32_t memoryBits=(1<<i);
        const bool isRequiredMemType=memReq.memoryTypeBits&memoryBits;
        const VkMemoryPropertyFlags propFlags=memoryProperties.memoryTypes[i].propertyFlags;
        const bool hasRequiredProperties=(propFlags&properties)==properties;
        if(isRequiredMemType&&hasRequiredProperties){
            memoryIndex=i;
            break;
        }else if(i==memoryProperties.memoryTypeCount-1){
            LOG.error("Couldn't find a suitable memory type for an allocation");
        }
        
    }
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize=memReq.size;
    allocInfo.sType=VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.memoryTypeIndex=memoryIndex;
    VkDeviceMemory deviceMemory;
    if(vkAllocateMemory(device,&allocInfo,ALLOCATOR,&deviceMemory)!=VK_SUCCESS){
        LOG.error("Failed to allocate memory");
    }
    return deviceMemory;
}
VkDevice Renderer::getDevice(){
    return device;
}
VkSurfaceFormatKHR Renderer::getSwapchainFormat(){
    return swapchainFormat;
}
VkExtent2D Renderer::getExtent2D(){
    return swapchainExtent;
}
VkExtent3D Renderer::getExtent3D(){
    VkExtent3D extent;
    extent.depth=1;
    extent.height=swapchainExtent.height;
    extent.width=swapchainExtent.width;
    return extent;
}
uint32_t Renderer::getGraphicsQueueFamilyIndex(){
    return graphicsQueueFamily.index;
}
VkImageView* Renderer::getSwapchainImageViews(){
    return swapchainImageViews;
}
void Renderer::terminate(){
    if(descriptorPool!=nullptr)
        vkDestroyDescriptorPool(device,*descriptorPool,ALLOCATOR);
    vkDestroyCommandPool(device,*cmdPool,ALLOCATOR);
    for(uint32_t i=0;i<swapchainImagesCount;++i)
        vkDestroyImageView(device,swapchainImageViews[i],ALLOCATOR);
    vkDestroySwapchainKHR(device,swapchain,ALLOCATOR);
    vkDestroyDevice(device,ALLOCATOR);
    vkDestroySurfaceKHR(vulkanInstance,surface,ALLOCATOR);
    vkDestroyInstance(vulkanInstance,ALLOCATOR);
    if(graphicsQueueFamily.index==presentQueueFamily.index){
        delete[] graphicsQueues;
    }else{
        //TODO
    }
    delete cmdPool;
    if(descriptorPool!=nullptr)
        delete descriptorPool;
    //TODO check if deallocating pointers is needed 
}
Renderer::~Renderer(){}