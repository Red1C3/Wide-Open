/*MIT License

Copyright (c) 2021 Mohammad Issawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#include <Helpers.h>
using namespace std;
VkPhysicalDevice Common::getPhysicalDevice(VkInstance instance)
{
    uint32_t physicalDevicesCount;
    vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, nullptr);
    VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[physicalDevicesCount];
    vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, physicalDevices);
    VkPhysicalDevice device;
    for (uint32_t i = 0; i < physicalDevicesCount; i++)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            device = physicalDevices[i];
            delete[] physicalDevices;
            break;
        }
    }
    Log::instance().log("Found a discrete device");
    return device;
}
VkCommandPool *Common::createCommandPool(VkDevice &device, uint32_t queueIndex)
{
    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = queueIndex;
    VkCommandPool *cmdPool = new VkCommandPool;
    if (vkCreateCommandPool(device, &createInfo, nullptr, cmdPool) != VK_SUCCESS)
    {
        return nullptr;
    }
    return cmdPool;
}
char *Common::readBinFile(const char *path, uint32_t &size)
{
    fstream fileStream(path, ios::binary | ios::ate | ios::in);
    char *data;
    if (fileStream.is_open())
    {
        size = fileStream.tellg();
        fileStream.seekg(0, ios::beg);
        data = new char[size];
        fileStream.read(data, size);
    }
    else
    {
        Log::instance().error("Failed to open bin file at", false);
        Log::instance().error(path);
    }
    return data;
}