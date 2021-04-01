#include<Helpers.h>
VkPhysicalDevice Common::getPhysicalDevice(VkInstance instance){
    uint32_t physicalDevicesCount;
    vkEnumeratePhysicalDevices(instance,&physicalDevicesCount,nullptr);
    VkPhysicalDevice* physicalDevices=new VkPhysicalDevice[physicalDevicesCount];
    vkEnumeratePhysicalDevices(instance,&physicalDevicesCount,physicalDevices);
    VkPhysicalDevice device;
    for(uint32_t i=0;i<physicalDevicesCount;i++){
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i],&properties);
        if(properties.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            device=physicalDevices[i];
            delete[] physicalDevices;
            break;
        }
    }
    Log::instance().log("Found a discrete device");
    return device;
}
VkCommandPool* Common::createCommandPool(VkDevice& device,uint32_t queueIndex){
    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex=queueIndex;
    VkCommandPool* cmdPool=new VkCommandPool;
    if(vkCreateCommandPool(device,&createInfo,nullptr,cmdPool)!=VK_SUCCESS){
        return nullptr;
    }
    return cmdPool;
}