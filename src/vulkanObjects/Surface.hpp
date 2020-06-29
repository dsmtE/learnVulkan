#pragma once 

#include "Instance.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Surface {
private:

	VkSurfaceKHR surface_;
	const VkInstance& instance_r;
public:
	Surface(const VkInstance& instance, GLFWwindow* window);
	~Surface();

	inline VkSurfaceKHR& getVkSUrface() { return surface_; }
};