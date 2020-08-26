#pragma once

#include "Vulkan.hpp"

#include <ostream>

namespace Vulkan {

		inline const char* toString(const VkPhysicalDeviceType deviceType) {
			switch (deviceType) {
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				return "Other";
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				return "Integrated GPU";
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				return "Discrete GPU";
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				return "Virtual GPU";
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				return "CPU";
			default:
				return "UnknownDeviceType";
			}
		}

		inline const char* toString(const uint32_t vendorId) {
			switch (vendorId) {
			case 0x1002:
				return "AMD";
			case 0x1010:
				return "ImgTec";
			case 0x10DE:
				return "NVIDIA";
			case 0x13B5:
				return "ARM";
			case 0x5143:
				return "Qualcomm";
			case 0x8086:
				return "INTEL";
			default:
				return "UnknownVendor";
			}
		}

		inline const char* toString(const VkObjectType objectType) {
			switch (objectType) {
#define STR(ot) case VK_OBJECT_TYPE_##ot: return #ot
				STR(UNKNOWN);
				STR(INSTANCE);
				STR(PHYSICAL_DEVICE);
				STR(DEVICE);
				STR(QUEUE);
				STR(SEMAPHORE);
				STR(COMMAND_BUFFER);
				STR(FENCE);
				STR(DEVICE_MEMORY);
				STR(BUFFER);
				STR(IMAGE);
				STR(EVENT);
				STR(QUERY_POOL);
				STR(BUFFER_VIEW);
				STR(IMAGE_VIEW);
				STR(SHADER_MODULE);
				STR(PIPELINE_CACHE);
				STR(PIPELINE_LAYOUT);
				STR(RENDER_PASS);
				STR(PIPELINE);
				STR(DESCRIPTOR_SET_LAYOUT);
				STR(SAMPLER);
				STR(DESCRIPTOR_POOL);
				STR(DESCRIPTOR_SET);
				STR(FRAMEBUFFER);
				STR(COMMAND_POOL);
				STR(SAMPLER_YCBCR_CONVERSION);
				STR(DESCRIPTOR_UPDATE_TEMPLATE);
				STR(SURFACE_KHR);
				STR(SWAPCHAIN_KHR);
				STR(DISPLAY_KHR);
				STR(DISPLAY_MODE_KHR);
				STR(DEBUG_REPORT_CALLBACK_EXT);
				STR(DEBUG_UTILS_MESSENGER_EXT);
				STR(ACCELERATION_STRUCTURE_KHR);
				STR(VALIDATION_CACHE_EXT);
				STR(PERFORMANCE_CONFIGURATION_INTEL);
				STR(DEFERRED_OPERATION_KHR);
				STR(INDIRECT_COMMANDS_LAYOUT_NV);
#undef STR
			default: return "unknown";
			}
		}

		inline const char* toString(const VkPresentModeKHR presenteMode) {
			switch (presenteMode) {
#define STR(pm) case VK_PRESENT_MODE_##pm: return #pm
				STR(IMMEDIATE_KHR);
				STR(MAILBOX_KHR);
				STR(FIFO_KHR);
				STR(FIFO_RELAXED_KHR);
				STR(SHARED_DEMAND_REFRESH_KHR);
				STR(SHARED_CONTINUOUS_REFRESH_KHR);
#undef STR
			default: return "unknown";
			}
		}

#define OSTREAM_OPERTOR(type, name) inline std::ostream& operator << (std::ostream& out, const type name) { return out << toString(name); }
		OSTREAM_OPERTOR(VkPhysicalDeviceType, deviceType)
		OSTREAM_OPERTOR(VkObjectType, objectType)
		OSTREAM_OPERTOR(VkPresentModeKHR, presenteMode)
#undef OSTREAM_OPERTOR
}
