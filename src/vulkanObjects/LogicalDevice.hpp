#pragma once

#include "PhysicalDevice.hpp"
#include "Instance.hpp"

#include <vulkan/vulkan.h>

class LogicalDevice {

private:
    const PhysicalDevice& physicalDevice_r;
    VkDevice logicalDevice_;

    VkQueue graphicsQueue_; // Implicitly deleted with the logical device
    VkQueue presentQueue_; // Implicitly deleted with the logical device

public:
    LogicalDevice(const PhysicalDevice& physicalDevice);
    ~LogicalDevice();

    inline VkDevice& getVkDevice() { return logicalDevice_; };
    inline const VkDevice& getVkDevice() const { return logicalDevice_; };

    inline VkQueue& graphicsQueue() { return graphicsQueue_; };
    inline const VkQueue& graphicsQueue() const  { return graphicsQueue_; };

    inline VkQueue& presentQueue() { return presentQueue_; };
    inline const VkQueue& presentQueue() const { return presentQueue_; };
};
