#pragma once

#include "Vulkan.hpp"

namespace Vulkan {

	class Device;

	class PipelineLayout final {
	public:

		VULKAN_NON_COPIABLE(PipelineLayout)

		PipelineLayout(const Device& device);
		~PipelineLayout();

	private:

		const Device& device_;

		VULKAN_HANDLE(VkPipelineLayout, pipelineLayout_)
	};

}
