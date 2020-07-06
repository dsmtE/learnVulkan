#include "Surface.hpp"

#include <stdexcept>

Surface::Surface(const VkInstance& instance, GLFWwindow* window) : instance_r{ instance } {

	if (glfwCreateWindowSurface(instance, window, nullptr, &surface_) != VK_SUCCESS) {
		throw std::runtime_error("Window surface creation failed!");
	}
}

Surface::~Surface() {
	vkDestroySurfaceKHR(instance_r, surface_, nullptr);
}
