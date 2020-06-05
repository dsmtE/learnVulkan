#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "vulkanObjects/Instance.hpp"

class App {

private:
    const std::string name_;
    const uint32_t width_;
    const uint32_t height_;

    GLFWwindow* window_;

    // vulkan objects
    Instance* instance_;

public:
    App(const char* name, const  uint32_t& width = 800, const uint32_t& height = 600);
    void run();

private:
    void initWindow();
    void mainLoop();
    void cleanup();
};