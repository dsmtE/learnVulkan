#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.hpp"

class Surface {
private:

	VkSurfaceKHR _surface;
public:
	Surface(const VkInstance& instance);
	~Surface();

	inline VkSurfaceKHR& getVkSUrface() { return _surface; }

private:

};