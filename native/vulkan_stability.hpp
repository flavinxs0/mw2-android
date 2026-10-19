#pragma once

#include <vulkan/vulkan.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace android_vulkan_stability {

struct DeviceReport {
    std::string text;
    bool supports_bc_compression = false;
    bool has_memory_budget_extension = false;
};

inline bool has_device_extension(VkPhysicalDevice physical_device,
                                 const char* wanted) {
    if (physical_device == VK_NULL_HANDLE || wanted == nullptr) return false;

    uint32_t count = 0;
    if (vkEnumerateDeviceExtensionProperties(
            physical_device, nullptr, &count, nullptr) != VK_SUCCESS) {
        return false;
    }

    std::vector<VkExtensionProperties> extensions(count);
    if (count != 0 &&
        vkEnumerateDeviceExtensionProperties(
            physical_device, nullptr, &count, extensions.data()) != VK_SUCCESS) {
        return false;
    }

    return std::any_of(extensions.begin(), extensions.end(),
        [wanted](const VkExtensionProperties& ext) {
            return std::strcmp(ext.extensionName, wanted) == 0;
        });
}

inline DeviceReport describe_device(VkPhysicalDevice physical_device) {
    DeviceReport result;
    if (physical_device == VK_NULL_HANDLE) {
        result.text = "Vulkan physical device handle is null.";
        return result;
    }

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physical_device, &properties);

    VkPhysicalDeviceFeatures features{};
    vkGetPhysicalDeviceFeatures(physical_device, &features);
    result.supports_bc_compression = features.textureCompressionBC == VK_TRUE;
#ifdef VK_EXT_memory_budget
    result.has_memory_budget_extension =
        has_device_extension(physical_device, VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
#endif

    std::ostringstream out;
    out << "Device: " << properties.deviceName << '\n'
        << "Vendor ID: 0x" << std::hex << properties.vendorID << '\n'
        << "Device ID: 0x" << properties.deviceID << '\n'
        << "Driver version: 0x" << properties.driverVersion << '\n'
        << "Vulkan API: " << VK_VERSION_MAJOR(properties.apiVersion) << '.'
        << VK_VERSION_MINOR(properties.apiVersion) << '.'
        << VK_VERSION_PATCH(properties.apiVersion) << '\n'
        << std::dec
        << "BC texture compression: "
        << (result.supports_bc_compression ? "supported" : "not reported") << '\n'
        << "VK_EXT_memory_budget: "
        << (result.has_memory_budget_extension ? "available" : "unavailable") << '\n';

    VkPhysicalDeviceMemoryProperties memory{};
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory);
    out << "Memory heaps: " << memory.memoryHeapCount << '\n';

#ifdef VK_EXT_memory_budget
    VkPhysicalDeviceMemoryBudgetPropertiesEXT budget{};
    budget.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT;
    VkPhysicalDeviceMemoryProperties2 memory2{};
    memory2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
    const bool can_query_budget = result.has_memory_budget_extension;
    if (can_query_budget) {
        memory2.pNext = &budget;
        vkGetPhysicalDeviceMemoryProperties2(physical_device, &memory2);
    }
#endif

    for (uint32_t i = 0; i < memory.memoryHeapCount; ++i) {
        const VkMemoryHeap& heap = memory.memoryHeaps[i];
        out << "  Heap " << i << ": size="
            << (heap.size / (1024 * 1024)) << " MiB"
            << ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) ? " device-local" : "");
#ifdef VK_EXT_memory_budget
        if (can_query_budget) {
            out << ", budget=" << (budget.heapBudget[i] / (1024 * 1024))
                << " MiB, usage=" << (budget.heapUsage[i] / (1024 * 1024))
                << " MiB";
        }
#endif
        out << '\n';
    }

    result.text = out.str();
    return result;
}

// If the driver rejects saved cache data, retry creation with an empty cache.
// The caller remains responsible for device lifecycle and synchronization.
inline VkResult create_pipeline_cache_safe(
    VkDevice device,
    const std::vector<uint8_t>& saved_bytes,
    VkPipelineCache* out_cache,
    bool* used_saved_cache = nullptr) {
    if (out_cache == nullptr || device == VK_NULL_HANDLE) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    *out_cache = VK_NULL_HANDLE;
    if (used_saved_cache) *used_saved_cache = false;

    VkPipelineCacheCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    if (!saved_bytes.empty() &&
        saved_bytes.size() <= static_cast<size_t>(UINT32_MAX)) {
        info.initialDataSize = saved_bytes.size();
        info.pInitialData = saved_bytes.data();
        VkResult result = vkCreatePipelineCache(device, &info, nullptr, out_cache);
        if (result == VK_SUCCESS) {
            if (used_saved_cache) *used_saved_cache = true;
            return VK_SUCCESS;
        }
        *out_cache = VK_NULL_HANDLE;
    }

    info.initialDataSize = 0;
    info.pInitialData = nullptr;
    return vkCreatePipelineCache(device, &info, nullptr, out_cache);
}

// Export cache data after checking Vulkan return codes and a defensive size cap.
// Do not call while another thread is mutating the same cache.
inline VkResult export_pipeline_cache(VkDevice device,
                                      VkPipelineCache cache,
                                      std::vector<uint8_t>* out_bytes) {
    if (device == VK_NULL_HANDLE || cache == VK_NULL_HANDLE ||
        out_bytes == nullptr) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    size_t size = 0;
    VkResult result = vkGetPipelineCacheData(device, cache, &size, nullptr);
    if (result != VK_SUCCESS) return result;
    if (size == 0 || size > (64u * 1024u * 1024u)) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    std::vector<uint8_t> bytes(size);
    result = vkGetPipelineCacheData(device, cache, &size, bytes.data());
    if (result != VK_SUCCESS) return result;
    if (size > bytes.size()) return VK_INCOMPLETE;

    bytes.resize(size);
    *out_bytes = std::move(bytes);
    return VK_SUCCESS;
}

}  // namespace android_vulkan_stability
