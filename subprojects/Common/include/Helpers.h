#pragma once
#include<vulkan/vulkan.hpp>
#include<Log.h>
namespace Common{
    VkPhysicalDevice getPhysicalDevice(VkInstance instance);
    VkCommandPool* createCommandPool(VkDevice& device,uint32_t queueIndex);
}