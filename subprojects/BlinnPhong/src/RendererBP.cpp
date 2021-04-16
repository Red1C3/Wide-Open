#include<RendererBP.h>
using namespace WideOpenBP;
using namespace Common;
RendererBP::RendererBP(){}
Assimp::Importer RendererBP::importer;
static inline VkSurfaceFormatKHR getSuitableFormat(uint32_t surfaceFormatsCount,VkSurfaceFormatKHR* formats){
    return formats[0];//TODO LOOK FOR THE BEST FORMAT FIRST
}
void RendererBP::init(){
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
    createDescriptorPool();
}
void RendererBP::createInstance(){
    VkApplicationInfo appInfo{};
    appInfo.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion=VK_API_VERSION_1_2;
    appInfo.applicationVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.engineVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.pApplicationName="Wide-OpenBP";
    appInfo.pEngineName="Wide-Open";
    VkInstanceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo=&appInfo;
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
/*IF YOUR MACHINE ONLY HAS AN INTEGRATED GPU (THAT SUPPORTS VULKAN) CHANGE THE CONDITION IN THE LOOP TO
VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU*/
void RendererBP::createPhysicalDevice(){
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
    vkGetPhysicalDeviceMemoryProperties(physicalDevice,&memProperties);
    
}
void RendererBP::createSurface(){
    if(glfwCreateWindowSurface(vkInstance,WINDOW.getWindow(),ALLOCATOR,&surface)!=VK_SUCCESS)
        LOG.error("Failed to get surface");
}
void RendererBP::createDevice(){
    VkDeviceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.enabledLayerCount=layersCount;
    createInfo.enabledExtensionCount=1;
    const char* extensions[]={VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.ppEnabledExtensionNames=extensions;
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
void RendererBP::createSwapchain(){
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
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,nullptr);
    VkImage* swapchainImages=new VkImage[swapchainImagesCount];
    vkGetSwapchainImagesKHR(device,swapchain,&swapchainImagesCount,swapchainImages);
    this->swapchainImages=new VkImageView[swapchainImagesCount];
    for(uint32_t i=0;i<swapchainImagesCount;i++){
        VkImageViewCreateInfo imgViewCreateInfo{};
        imgViewCreateInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.format=swapchainFormat.format;
        imgViewCreateInfo.image=swapchainImages[i];
        imgViewCreateInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imgViewCreateInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
        imgViewCreateInfo.subresourceRange.baseArrayLayer=0;
        imgViewCreateInfo.subresourceRange.baseMipLevel=0;
        imgViewCreateInfo.subresourceRange.layerCount=1;
        imgViewCreateInfo.subresourceRange.levelCount=1;
        imgViewCreateInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
        if(vkCreateImageView(device,&imgViewCreateInfo,ALLOCATOR,&(this->swapchainImages[i]))!=VK_SUCCESS){
            LOG.error("Failed to create swapchain image view");
        }
    }
    delete[] surfaceFormats;
    delete[] swapchainImages;
}
void RendererBP::createDescriptorPool(){
    VkDescriptorPoolSize poolSize{};
    poolSize.descriptorCount=MAX_MESHES;
    poolSize.type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets=MAX_MESHES;
    createInfo.poolSizeCount=1;
    createInfo.pPoolSizes=&poolSize;
    if(vkCreateDescriptorPool(DEVICE,&createInfo,ALLOCATOR,&descriptorPool)!=VK_SUCCESS){
        LOG.error("Failed to create descriptorPool");
    }
}
VkDeviceMemory RendererBP::allocateMemory(VkMemoryRequirements memReq,VkMemoryPropertyFlags properties){
    uint32_t memoryIndex;
    for(uint32_t i=0;i<memProperties.memoryTypeCount;i++){
        const uint32_t memoryBits=(1<<i);
        const bool isRequiredMemType=memReq.memoryTypeBits&memoryBits;
        const VkMemoryPropertyFlags propFlags=memProperties.memoryTypes[i].propertyFlags;
        const bool hasRequiredProperties=(propFlags&properties)==properties;
        if(isRequiredMemType&&hasRequiredProperties){
            memoryIndex=i;
            break;
        }else if(i==memProperties.memoryTypeCount-1){
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
VkDevice& RendererBP::getDevice(){
    return device;
}
VkPhysicalDeviceMemoryProperties& RendererBP::getMemProperties(){
    return memProperties;
}
VkSurfaceFormatKHR RendererBP::getSwapchainFormat(){
    return swapchainFormat;
}
VkExtent2D RendererBP::getExtent(){
    return surfaceCapabilities.currentExtent;
}
uint32_t RendererBP::getGraphicsQueueIndex(){
    return queueIndex;
}
VkImageView* RendererBP::getSwapchainImgViews(){
    return swapchainImages;
}
VkCommandPool RendererBP::getCmdPool(){
    return *cmdPool;
}
VkSwapchainKHR RendererBP::getSwapchain(){
    return swapchain;
}
uint32_t RendererBP::getSwapchainImagesCount(){
    return swapchainImagesCount;
}
void RendererBP::terminate(){
    vkDestroyDescriptorPool(DEVICE,descriptorPool,ALLOCATOR);
    vkDestroyCommandPool(device,*cmdPool,ALLOCATOR);
    for(uint32_t i=0;i<swapchainImagesCount;i++)
        vkDestroyImageView(device,swapchainImages[i],ALLOCATOR);
    vkDestroySwapchainKHR(device,swapchain,ALLOCATOR);
    vkDestroyDevice(device,ALLOCATOR);
    vkDestroySurfaceKHR(vkInstance,surface,ALLOCATOR);
    vkDestroyInstance(vkInstance,ALLOCATOR);
    delete cmdPool;
}
void RendererBP::allocateDescriptorSet(VkDescriptorSet* set){
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool=descriptorPool;
    allocInfo.descriptorSetCount=1;
    VkDescriptorSetLayout layout=LAYOUT.getLayout();
    allocInfo.pSetLayouts=&layout;
    if(vkAllocateDescriptorSets(DEVICE,&allocInfo,set)!=VK_SUCCESS){
        LOG.error("Failed to allocate Descriptor set");
    }
}
RendererBP& RendererBP::instance(){
    static RendererBP renderer;
    return renderer;
}