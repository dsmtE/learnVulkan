#include "LogicalDevice.hpp"

#include <set>
#include <stdexcept>

LogicalDevice::LogicalDevice(const PhysicalDevice& physicalDevice) : logicalDevice_{ VK_NULL_HANDLE }, physicalDevice_r{ physicalDevice } {
	const QueueFamilyIndices& indices = physicalDevice_r.getQueueFamilyIndices();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(PhysicalDevice::neededDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = PhysicalDevice::neededDeviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice_r.getVkPhysicalDevice(), &createInfo, nullptr, &logicalDevice_) != VK_SUCCESS) {
		throw std::runtime_error("[LogicalDevice] failure to create a logical device !");
	}
	// store Queue references 
	vkGetDeviceQueue(logicalDevice_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(logicalDevice_, indices.presentFamily.value(), 0, &presentQueue_);

}

LogicalDevice::~LogicalDevice() {
	vkDestroyDevice(logicalDevice_, nullptr);
}
