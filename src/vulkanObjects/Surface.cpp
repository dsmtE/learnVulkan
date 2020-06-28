#include "Surface.hpp"
#include <stdexcept>

Surface::Surface(const VkInstance& instance, GLFWwindow* window) {

	if (glfwCreateWindowSurface(instance, window, nullptr, &_surface) != VK_SUCCESS) {
		throw std::runtime_error("Window surface creation failed!");
	}
}

Surface::~Surface() {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
