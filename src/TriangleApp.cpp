#include "TriangleApp.hpp"

namespace
{
	const bool EnableValidationLayers =
#ifdef NDEBUG
		false;
#else
		true;
#endif
}

TriangleApp::TriangleApp(const Vulkan::WindowConfig& windowConfig) :
	Vulkan::Application("Vulkan triangle test", windowConfig, VK_PRESENT_MODE_MAILBOX_KHR, EnableValidationLayers)
{}

TriangleApp::~TriangleApp() {
	TriangleApp::DeleteSwapChain();
}
