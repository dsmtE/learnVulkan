#pragma once

#include <optional>
#include <vulkan/vulkan.h>
#include <map>
#include <vector>

#include "Instance.hpp"


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() { return graphicsFamily.has_value();  }
};

class Devices {

private:
    VkPhysicalDevice physicalDevice_;
    VkDevice logicalDevice_;
    VkQueue graphicsQueue_;

public:
    Devices(const VkInstance& instance);
    ~Devices();

    inline VkQueue& getGraphicsQueue() { return graphicsQueue_; };

private:
    // check if GPU is suiable and return rating throught reference
    bool isPhysicalDeviceSuitable(VkPhysicalDevice device, int& score);
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

};
