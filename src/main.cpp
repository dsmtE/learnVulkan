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

	void PrintVulkanSdkInformation() {
		std::cout << "Vulkan SDK Header Version: " << VK_HEADER_VERSION << std::endl;
	}

	void PrintVulkanInstanceInformation(const Vulkan::Application& application) {

		std::cout << "Vulkan Instance Extensions: " << std::endl;

		for (const auto& extension : application.Extensions()) {
			std::cout << "- " << extension.extensionName << " (" << Vulkan::Version(extension.specVersion) << ")" << std::endl;
		}
	}

	void PrintVulkanDevices(const Vulkan::Application& application) {
		std::cout << "Vulkan Devices: " << std::endl;

		for (const auto& device : application.PhysicalDevices()) {
			VkPhysicalDeviceProperties prop;
			vkGetPhysicalDeviceProperties(device, &prop);

			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(device, &features);

			const Vulkan::Version vulkanVersion(prop.apiVersion);
			const Vulkan::Version driverVersion(prop.driverVersion, prop.vendorID);

			std::cout << "- [" << prop.deviceID << "] ";
			std::cout << Vulkan::toString(prop.vendorID) << " '" << prop.deviceName;
			std::cout << "' (";
			std::cout << prop.deviceType << ": ";
			std::cout << "vulkan " << vulkanVersion << ", ";
			std::cout << "driver " << driverVersion;
			std::cout << ")" << std::endl;
		}
	}

	void PrintVulkanSwapChainInformation(const Vulkan::Application& application) {
		const Vulkan::SwapChain& swapChain = application.SwapChain();

		std::cout << "Swap Chain: " << std::endl;
		std::cout << "- image count: " << swapChain.Images().size() << std::endl;
		std::cout << "- present mode: " << Vulkan::toString(swapChain.PresentMode()) << std::endl;
	}

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

		PrintVulkanSdkInformation();
		PrintVulkanInstanceInformation(application);
		PrintVulkanDevices(application);
		SetVulkanDevice(application);
		PrintVulkanSwapChainInformation(application);

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
