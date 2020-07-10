#pragma once 

#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "Surface.hpp"

#include <vulkan/vulkan.h>

#include <vector>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain {
private:

	VkSwapchainKHR swapChain_;
	std::vector<VkImage> imagesBuffer_;
	std::vector<VkImageView> imagesViewsBuffer_;
	VkFormat swapChainImageFormat_;
	VkExtent2D swapChainExtent_;

	const VkDevice device_;
public:
	SwapChain(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const Surface& surface);
	~SwapChain();

	static SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
	void createImageViews();
};