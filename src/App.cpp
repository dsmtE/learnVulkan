#include "App.hpp"

#include <iostream>

App::App(const char* name, const uint32_t& width, const uint32_t& height) : name_{ name }, width_{ width }, height_{ height }, window_{ nullptr }, instance_{ nullptr } {
    initWindow();

    // init vulkan
    try {
        instance_ = new Instance("Vulkan app");
        _surface = new Surface(instance_->getVkInstance(), window_);
        devices_ = new Devices(instance_->getVkInstance(), _surface->getVkSUrface());

    } catch (const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    }

}

App::~App() {
    delete _surface;
    delete devices_;
    delete instance_;

    glfwDestroyWindow(window_);

    glfwTerminate();
}

void App::run() {
    mainLoop();
}


void App::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window_ = glfwCreateWindow(width_, height_, name_.c_str(), nullptr, nullptr);
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
    }
}