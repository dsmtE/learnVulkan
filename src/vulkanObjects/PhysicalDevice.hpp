#pragma once

#include "Instance.hpp"

#include <vulkan/vulkan.h>

#include <optional>
#include <map>
#include <vector>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value();  }
};

class PhysicalDevice {

private:
    VkPhysicalDevice physicalDevice_; // Implicitly deleted with instance
    QueueFamilyIndices queueFamilyIndices_; // Used to store familyIndices of the selected device

    const VkInstance& instance_r;
    const VkSurfaceKHR& surface_r;

public:
    PhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
    ~PhysicalDevice();

    inline VkPhysicalDevice& getVkPhysicalDevice() { return physicalDevice_; }
    inline const VkPhysicalDevice& getVkPhysicalDevice() const { return physicalDevice_; }

    inline QueueFamilyIndices& getQueueFamilyIndices() { return queueFamilyIndices_; };
    inline const QueueFamilyIndices& getQueueFamilyIndices() const { return queueFamilyIndices_; };

private:
    // Check if GPU is suitable and return rating throught reference
    static bool isPhysicalDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, int& score);
    // Get needed families queue indices
    static QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

};
