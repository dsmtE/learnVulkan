#include "TriangleApp.hpp"

#include "Vulkan/Version.hpp"
#include "Vulkan/WindowConfig.hpp"
#include "Vulkan/Enumerate.hpp"
#include "Vulkan/Strings.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Strings.hpp"

#include <Utilities/Console.hpp>

#include <stdexcept>
#include <iostream>


namespace {

	void SetVulkanDevice(Vulkan::Application& application) {
		const auto& physicalDevices = application.PhysicalDevices();
		const auto result = std::find_if(physicalDevices.begin(), physicalDevices.end(), [](const VkPhysicalDevice& device)
			{
				// We want a device with geometry shader support
				VkPhysicalDeviceFeatures deviceFeatures;
				vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

				if (!deviceFeatures.geometryShader)
					return false;

				// We want a device with a graphics queue
				const auto queueFamilies = Vulkan::GetEnumerateVector(device, vkGetPhysicalDeviceQueueFamilyProperties);
				const auto hasGraphicsQueue = std::find_if(queueFamilies.begin(), queueFamilies.end(), [](const VkQueueFamilyProperties& queueFamily)
					{
						return queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
					});

				return hasGraphicsQueue != queueFamilies.end();
			});

		if (result == physicalDevices.end())
			throw std::runtime_error("cannot find a suitable device");

		application.SetPhysicalDevice(*result);
	}
}

int main(int argc, const char* argv[]) noexcept {
	try {
		const Vulkan::WindowConfig windowConfig
		{
			"Vulkan Triangle",
			1280,
			720,
			false,
			false,
			true
		};

		TriangleApp application(windowConfig);

		SetVulkanDevice(application);

		application.Run();

		return EXIT_SUCCESS;
	
	}

	catch (const std::exception& exception) {
		Utilities::Console::Write(Utilities::Severity::Fatal, [&exception]()
			{
				std::cerr << "FATAL: " << exception.what() << std::endl;
			});
	}

	catch (...)
	{
		Utilities::Console::Write(Utilities::Severity::Fatal, []()
			{
				std::cerr << "FATAL: caught unhandled exception" << std::endl;
			});
	}
	

	return EXIT_FAILURE;
}

