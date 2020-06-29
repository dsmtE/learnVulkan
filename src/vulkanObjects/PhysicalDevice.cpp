#include "PhysicalDevice.hpp"

#include <iostream>
#include <set>

PhysicalDevice::PhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface) : physicalDevice_{ VK_NULL_HANDLE }, instance_r{ instance }, surface_r{ surface } {

	#ifndef NDEBUG
		std::cout << "[vulkan initialisation] PhysicalDevice initialisation " << std::endl;
	#endif

	// Get physical devices count
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("[PhysicalDevices] Error: No graphics card support the Vulkan API !");
	}

	// get devices references
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// The use of a map allows us to sort the devices automatically in an ascending order.
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		int score = 0;
		if (isPhysicalDeviceSuitable(device, surface_r, score)) { // add only suitable devices
			candidates.insert(std::make_pair(score, device));
		}	
	}

	// TODO display the score and let the user choose the GPU manually
	if (candidates.rbegin()->first > 0) {
		// select our physical device
		physicalDevice_ = candidates.rbegin()->second;
		// save queue family indices
		queueFamilyIndices_ = findQueueFamilies(physicalDevice_, surface_r);
		#ifndef NDEBUG
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice_, &deviceProperties);
				std::cout << "[PhysicalDevice] selected device: " <<  deviceProperties.deviceName << " apiVersion: " << deviceProperties.apiVersion << " driverVersion: " << deviceProperties.driverVersion << std::endl;
		#endif

	} else {
		throw std::runtime_error("[PhysicalDevices] Error: No graphics card can run this program !");
	}

}

PhysicalDevice::~PhysicalDevice() {
}


bool PhysicalDevice::isPhysicalDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, int& score) {

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (indices.isComplete()) { // suitable
		score = 0;

		// Dedicated graphics cards have a huge performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 1000; }

		// The maximum size of the textures affects their quality
		score += deviceProperties.limits.maxImageDimension2D;

		return true;
	}

	return false;
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}