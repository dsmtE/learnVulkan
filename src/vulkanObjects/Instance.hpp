#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

class Instance {

private:
    VkInstance instance_;
    VkDebugUtilsMessengerEXT callback_;

public:
	Instance (const char* applicationName);
	~Instance ();

    // VKAPI_ATTR & VKAPI_CALL here for compiler compatibilities
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback( 
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pCallback);

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

private:
    void setupDebugMessenger();
    void checkExtensions(const std::vector<const char*>& requiredExtensionsNames);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
};