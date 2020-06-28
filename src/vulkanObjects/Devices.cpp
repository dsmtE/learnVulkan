#include "Devices.hpp"

#include <iostream>
#include <set>

Devices::Devices(const VkInstance& instance, const VkSurfaceKHR& surface) : physicalDevice_{ VK_NULL_HANDLE }, logicalDevice_{ VK_NULL_HANDLE } {

	#ifndef NDEBUG
		std::cout << "[vulkan initialisation] Devices initialisation " << std::endl;
	#endif

	
	////////////////////////////// PhysicalDevices ////////////////////////////////

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("[PhysicalDevices] aucune carte graphique ne supporte Vulkan!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());


	// L'utilisation d'une map permet de les trier automatiquement de manière ascendante
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		int score = 0;
		if (isPhysicalDeviceSuitable(device, surface, score)) { // add only suitable devices
			candidates.insert(std::make_pair(score, device));
		}	
	}

	// TODO display the score and let the user choose the GPU manually
	if (candidates.rbegin()->first > 0) {
		physicalDevice_ = candidates.rbegin()->second;
		#ifndef NDEBUG
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice_, &deviceProperties);
				std::cout << "[PhysicalDevice] selected device: " <<  deviceProperties.deviceName << " apiVersion: " << deviceProperties.apiVersion << " driverVersion: " << deviceProperties.driverVersion << std::endl;
		#endif
	} else {
		throw std::runtime_error("aucun GPU ne peut executer ce programme!");
	}

	////////////////////////////// VirtualDevices ////////////////////////////////

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice_, surface);

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


	createInfo.enabledExtensionCount = 0;

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &logicalDevice_) != VK_SUCCESS) {
		throw std::runtime_error("[LogicalDevice] échec lors de la création d'un logical device!");
	}

	vkGetDeviceQueue(logicalDevice_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(logicalDevice_, indices.presentFamily.value(), 0, &_presentQueue);
}

Devices::~Devices() {

	vkDestroyDevice(logicalDevice_, nullptr);
}


bool Devices::isPhysicalDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, int& score) {

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (indices.isComplete()) { // suitable
		score = 0;

		// Les carte graphiques dédiées ont un énorme avantage en terme de performances
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 1000; }

		// La taille maximale des textures affecte leur qualité
		score += deviceProperties.limits.maxImageDimension2D;

		return true;
	}

	return false;
}

QueueFamilyIndices Devices::findQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface) {
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