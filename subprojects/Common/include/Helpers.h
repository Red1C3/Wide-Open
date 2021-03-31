#pragma once
#include<vulkan/vulkan.hpp>
#include<Log.h>
namespace Common{
    VkPhysicalDevice getPhysicalDevice(VkInstance instance);
    VkCommandPool* createCommandPool();
}