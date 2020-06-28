#include "Surface.hpp"

Surface(const VkInstance& instance) {
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window);
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("échec de la creation d'une window surface!");
	}
}

Surface::~Surface() {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
