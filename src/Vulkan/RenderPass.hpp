#pragma once

#include "Vulkan.hpp"

namespace Vulkan
{
	class DepthBuffer;
	class SwapChain;

	class RenderPass final
	{
	public:

		VULKAN_NON_COPIABLE(RenderPass)

		RenderPass(const SwapChain& swapChain, bool clearColorBuffer);
		~RenderPass();

		const class SwapChain& SwapChain() const { return swapChain_; }

	private:

		const class SwapChain& swapChain_;

		VULKAN_HANDLE(VkRenderPass, renderPass_)
	};

}
