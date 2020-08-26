#include "Application.hpp"


#include "CommandPool.hpp"
#include "CommandBuffers.hpp"
#include "DebugUtilsMessenger.hpp"
#include "Device.hpp"
#include "FrameBuffer.hpp"
#include "GraphicsPipeline.hpp"
#include "Instance.hpp"
#include "PipelineLayout.hpp"
#include "RenderPass.hpp"
#include "Semaphore.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "Window.hpp"
#include "Fence.hpp"
#include "Strings.hpp"

#include <array>
#include <stdexcept>

namespace Vulkan {

Application::Application(const char* applicationName, const WindowConfig& windowConfig, const VkPresentModeKHR presentMode, const bool enableValidationLayers) :
	presentMode_(presentMode), framebufferResized_(false)
{
	const auto validationLayers = enableValidationLayers
		? std::vector<const char*>{"VK_LAYER_KHRONOS_validation"}
		: std::vector<const char*>();

	window_.reset(new class Window(windowConfig));
	instance_.reset(new Instance(applicationName , *window_, validationLayers));
	debugUtilsMessenger_.reset(enableValidationLayers ? new DebugUtilsMessenger(*instance_, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) : nullptr);
	surface_.reset(new Surface(*instance_));
}

Application::~Application() {
	Application::DeleteSwapChain();

	commandPool_.reset();
	device_.reset();
	surface_.reset();
	debugUtilsMessenger_.reset();
	instance_.reset();
	window_.reset();
}

const std::vector<VkExtensionProperties>& Application::Extensions() const {
	return instance_->Extensions();
}

const std::vector<VkPhysicalDevice>& Application::PhysicalDevices() const {
	return instance_->PhysicalDevices();
}

void Application::SetPhysicalDevice(VkPhysicalDevice physicalDevice) {
	if (device_)
		throw std::logic_error("physical device has already been set");

	device_.reset(new class Device(physicalDevice, *surface_));
	commandPool_.reset(new class CommandPool(*device_, device_->GraphicsFamilyIndex(), true));

	OnDeviceSet();

	// Create swap chain and command buffers
	CreateSwapChain();
}

void Application::Run() {
	if (!device_)
		throw std::logic_error("physical device has not been set");

	currentFrame_ = 0;

	window_->DrawFrame = [this]() { DrawFrame(); };
	window_->OnKey = [this](const int key, const int scancode, const int action, const int mods) { OnKey(key, scancode, action, mods); };
	window_->OnCursorPosition = [this](const double xpos, const double ypos) { OnCursorPosition(xpos, ypos); };
	window_->OnMouseButton = [this](const int button, const int action, const int mods) { OnMouseButton(button, action, mods); };
	window_->OnScroll = [this](const double xoffset, const double yoffset) { OnScroll(xoffset, yoffset); };
	window_->OnFramebufferSize = [this](const int width, const int height) { OnFramebufferSize(width, height); };
	window_->Run();
	device_->WaitIdle();
}

void Application::CreateSwapChain() {
	// Wait until the window is visible
	while (window_->IsMinimized()) {
		window_->WaitForEvents();
	}

	device_->WaitIdle();

	swapChain_.reset(new class SwapChain(*device_, presentMode_));

	// Create Semaphores for each swapChain ImageViews and frame for CPU-GPU syncro
	for (size_t i = 0; i != swapChain_->ImageViews().size(); ++i) {
		imageAvailableSemaphores_.emplace_back(*device_);
		renderFinishedSemaphores_.emplace_back(*device_);
		inFlightFences_.emplace_back(*device_, true);
	}

	graphicsPipeline_.reset(new class GraphicsPipeline(*swapChain_, isWireFrame_));

	// FrameBuffers creation
	for (const auto& imageView : swapChain_->ImageViews()) {
		swapChainFramebuffers_.emplace_back(*imageView, graphicsPipeline_->RenderPass());
	}

	commandBuffers_.reset(new CommandBuffers(*commandPool_, static_cast<uint32_t>(swapChainFramebuffers_.size())));
}

void Application::DeleteSwapChain() {
	commandBuffers_.reset();
	swapChainFramebuffers_.clear();
	graphicsPipeline_.reset();
	inFlightFences_.clear();
	renderFinishedSemaphores_.clear();
	imageAvailableSemaphores_.clear();
	swapChain_.reset();
}

void Application::DrawFrame() {
	constexpr auto noTimeout = std::numeric_limits<uint64_t>::max();

	auto& inFlightFence = inFlightFences_[currentFrame_];
	const auto imageAvailableSemaphore = imageAvailableSemaphores_[currentFrame_].Handle();
	const auto renderFinishedSemaphore = renderFinishedSemaphores_[currentFrame_].Handle();

	inFlightFence.Wait(noTimeout);

	uint32_t imageIndex;
	auto result = vkAcquireNextImageKHR(device_->Handle(), swapChain_->Handle(), noTimeout, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || isWireFrame_ != graphicsPipeline_->IsWireFrame()) {
		RecreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error(std::string("failed to acquire next image (") + toString(result) + ")");
	}

	// TODO do not recreate commandBuffers
	const auto commandBuffer = commandBuffers_->Begin(imageIndex);
	Render(commandBuffer, imageIndex);
	commandBuffers_->End(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkCommandBuffer commandBuffers[]{ commandBuffer };
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	inFlightFence.Reset();

	Check(vkQueueSubmit(device_->GraphicsQueue(), 1, &submitInfo, inFlightFence.Handle()), "submit draw command buffer");

	VkSwapchainKHR swapChains[] = { swapChain_->Handle() };
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(device_->PresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized_) {
		framebufferResized_ = false;
		RecreateSwapChain();
		return;
	}
	
	if (result != VK_SUCCESS)
		throw std::runtime_error(std::string("failed to present next image (") + toString(result) + ")");

	currentFrame_ = (currentFrame_ + 1) % inFlightFences_.size();
}

void Application::Render(VkCommandBuffer commandBuffer, const uint32_t imageIndex) {
	std::array<VkClearValue, 1> clearValues = {};
	clearValues[0].color = { {1.0f, 0.0f, 0.0f, 1.0f} };

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = graphicsPipeline_->RenderPass().Handle();
	renderPassInfo.framebuffer = swapChainFramebuffers_[imageIndex].Handle();
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain_->Extent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_->Handle());

		uint32_t vertexOffset = 0;
		uint32_t vertexCount = 3;

		vkCmdDraw(commandBuffer, vertexCount, 1, vertexOffset, 0);
	}
	vkCmdEndRenderPass(commandBuffer);
}


void Application::OnFramebufferSize(int width, int height) {
	framebufferResized_ = true;
}
void Application::RecreateSwapChain() {
	device_->WaitIdle();
	DeleteSwapChain();
	CreateSwapChain();
}

}
