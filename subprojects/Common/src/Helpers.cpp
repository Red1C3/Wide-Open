#include<Helpers.h>
using namespace Common;
VkPhysicalDevice getPhysicalDevice(VkInstance instance){
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