#pragma once
#include<vulkan/vulkan.hpp>
#include<Log.h>
#include<fstream>
namespace Common{
    VkPhysicalDevice getPhysicalDevice(VkInstance instance);
    VkCommandPool* createCommandPool(VkDevice& device,uint32_t queueIndex);
    char* readBinFile(const char* path,uint32_t& size);
}