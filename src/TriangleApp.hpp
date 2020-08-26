#pragma once

#include "Vulkan/Application.hpp"

class TriangleApp : public Vulkan::Application {
public:

	VULKAN_NON_COPIABLE(TriangleApp);

	TriangleApp(const Vulkan::WindowConfig& windowConfig);
	~TriangleApp();
};

