#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "vulkanObjects/Instance.hpp"
#include "vulkanObjects/PhysicalDevice.hpp"
#include "vulkanObjects/LogicalDevice.hpp"
#include "vulkanObjects/Surface.hpp"

class App {

private:
    const std::string name_;
    const uint32_t width_;
    const uint32_t height_;

    GLFWwindow* window_;

    // vulkan objects
    Instance* instance_;
    PhysicalDevice* physicalDevice_;
    LogicalDevice* logicalDevice_;
    Surface* _surface;

public:
    App(const char* name, const  uint32_t& width = 800, const uint32_t& height = 600);
    ~App();
    void run();

private:
    void initWindow();
    void mainLoop();
};