#pragma once

#include <optional>
#include <vulkan/vulkan.h>
#include <map>
#include <vector>

#include "Instance.hpp"


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value();  }
};

class Devices {

private:
    VkPhysicalDevice physicalDevice_;
    VkDevice logicalDevice_;
    VkQueue graphicsQueue_;
    VkQueue _presentQueue;

public:
    Devices(const VkInstance& instance, const VkSurfaceKHR& surface);
    ~Devices();

    inline VkQueue& getGraphicsQueue() { return graphicsQueue_; };

private:
    // check if GPU is suiable and return rating throught reference
    bool isPhysicalDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, int& score);
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface);

};
