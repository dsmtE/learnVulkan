#pragma once

#include "Vulkan.hpp"

namespace Vulkan
{
	class Instance; // forward declaration

	class DebugUtilsMessenger final {
	public:

		VULKAN_NON_COPIABLE(DebugUtilsMessenger)

			DebugUtilsMessenger(const Instance& instance, VkDebugUtilsMessageSeverityFlagBitsEXT threshold);
		~DebugUtilsMessenger();

		VkDebugUtilsMessageSeverityFlagBitsEXT Threshold() const { return threshold_; }

	private:

		void populateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, const VkDebugUtilsMessageSeverityFlagsEXT severity);

		const Instance& instance_;
		const VkDebugUtilsMessageSeverityFlagBitsEXT threshold_;

		VULKAN_HANDLE(VkDebugUtilsMessengerEXT, messenger_)
	};
}
