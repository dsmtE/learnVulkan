#include "Instance.hpp"

#include "Enumerate.hpp"
#include "Version.hpp"
#include "Window.hpp"

#include <algorithm>
#include <sstream>

namespace Vulkan {

    Instance::Instance(const char* applicationName, const class Window& window, const std::vector<const char*>& validationLayers) :
        window_(window),
        validationLayers_(validationLayers)
    {
        // Check the minimum version.
        const uint32_t version = VK_API_VERSION_1_1;

        CheckVulkanMinimumVersion(version);

        // Get the list of required extensions.
        auto extensions = window.GetRequiredInstanceExtensions();

        // Check the validation layers and add them to the list of required extensions.
        CheckVulkanValidationLayerSupport(validationLayers);

        if (!validationLayers.empty())
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        // Create the Vulkan instance.
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = version;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        Check(vkCreateInstance(&createInfo, nullptr, &instance_), "create instance");

        GetVulkanDevices();
        GetVulkanExtensions();
        CheckVulkanInstanceExtensionsSupport();
    }

    Instance::~Instance() {
        if (instance_ != nullptr) {
            vkDestroyInstance(instance_, nullptr);
            instance_ = nullptr;
        }
    }

    void Instance::GetVulkanDevices() {
        GetEnumerateVector(instance_, vkEnumeratePhysicalDevices, physicalDevices_);

        if (physicalDevices_.empty())
            throw std::runtime_error("found no Vulkan physical devices");
    }

    void Instance::GetVulkanExtensions() {
        GetEnumerateVector(static_cast<const char*>(nullptr), vkEnumerateInstanceExtensionProperties, extensions_);
    }

    void Instance::CheckVulkanMinimumVersion(const uint32_t minVersion) {
        uint32_t version;
        Check(vkEnumerateInstanceVersion(&version), "query instance version");

        if (minVersion > version) {
            std::ostringstream out;
            out << "minimum required version not found (required " << Version(minVersion);
            out << ", found " << Version(version) << ")";

            throw std::runtime_error(out.str());
        }
    }

    void Instance::CheckVulkanValidationLayerSupport(const std::vector<const char*>& validationLayers)
    {
        const std::vector<VkLayerProperties> availableLayers = GetEnumerateVector(vkEnumerateInstanceLayerProperties);

        for (const char* layer : validationLayers) {
            auto result = std::find_if(availableLayers.begin(), availableLayers.end(), [layer](const VkLayerProperties& layerProperties)
                {
                    return strcmp(layer, layerProperties.layerName) == 0;
                });

            if (result == availableLayers.end())
                throw std::runtime_error("could not find the requested validation layer: '" + std::string(layer) + "'");
        }
    }

    void Instance::CheckVulkanInstanceExtensionsSupport() {

        // Get required extensions using glfw for Vulkan interface window
        // because vulkan doesn't know the platform he's working on...
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> neededExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // add validationLayers extensions
        neededExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        for (const char* extension : neededExtensions) {
            auto result = std::find_if(extensions_.begin(), extensions_.end(), [extension](const VkExtensionProperties& extensionProperties)
                {
                    return strcmp(extension, extensionProperties.extensionName) == 0;
                });

            if (result == extensions_.end())
                throw std::runtime_error("could not find the requested extension: '" + std::string(extension) + "'");
        }
    };
}