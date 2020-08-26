#pragma once

#include "FrameBuffer.hpp"
#include "WindowConfig.hpp"

#include <vector>
#include <memory>


namespace Vulkan {

	class Application {
	public:

		VULKAN_NON_COPIABLE(Application)

		virtual ~Application();

		const std::vector<VkExtensionProperties>& Extensions() const;
		const std::vector<VkPhysicalDevice>& PhysicalDevices() const;

		const class SwapChain& SwapChain() const { return *swapChain_; }
		class Window& Window() { return *window_; }
		const class Window& Window() const { return *window_; }

		bool HasSwapChain() const { return swapChain_.operator bool(); }

		void SetPhysicalDevice(VkPhysicalDevice physicalDevice);
		void Run();

	protected:

		Application(const char* applicationName, const WindowConfig& windowConfig, VkPresentModeKHR presentMode, bool enableValidationLayers);

		const class Device& Device() const { return *device_; }
		class CommandPool& CommandPool() { return *commandPool_; }
		const class GraphicsPipeline& GraphicsPipeline() const { return *graphicsPipeline_; }
		const class FrameBuffer& SwapChainFrameBuffer(const size_t i) const { return swapChainFramebuffers_[i]; }

		virtual void CreateSwapChain();
		virtual void DeleteSwapChain();
		virtual void DrawFrame();
		virtual void Render(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		virtual void OnDeviceSet() { };
		virtual void OnKey(int key, int scancode, int action, int mods) { }
		virtual void OnCursorPosition(double xpos, double ypos) { }
		virtual void OnMouseButton(int button, int action, int mods) { }
		virtual void OnScroll(double xoffset, double yoffset) { }

		void OnFramebufferSize( int width, int height);

		bool isWireFrame_{};

	private:

		void RecreateSwapChain();

		const VkPresentModeKHR presentMode_;
		
		std::unique_ptr<class Window> window_;
		std::unique_ptr<class Instance> instance_;
		std::unique_ptr<class DebugUtilsMessenger> debugUtilsMessenger_;
		std::unique_ptr<class Surface> surface_;
		std::unique_ptr<class Device> device_;
		std::unique_ptr<class SwapChain> swapChain_;
		std::unique_ptr<class GraphicsPipeline> graphicsPipeline_;
		std::vector<class FrameBuffer> swapChainFramebuffers_;
		std::unique_ptr<class CommandPool> commandPool_;
		std::unique_ptr<class CommandBuffers> commandBuffers_;
		std::vector<class Semaphore> imageAvailableSemaphores_;
		std::vector<class Semaphore> renderFinishedSemaphores_;
		std::vector<class Fence> inFlightFences_;

		size_t currentFrame_{};

		bool framebufferResized_;
	};

}
