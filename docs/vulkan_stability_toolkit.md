# Vulkan stability helpers for Android

This directory documents a small reusable C++ helper header for Vulkan diagnostics and pipeline-cache handling. It is intended to complement the app's existing renderer; it does not replace or automatically modify renderer behavior.

## Included helpers

- `describe_device(VkPhysicalDevice)`: reports GPU identity, driver/API versions, BC texture-compression feature support, memory heaps, and optional `VK_EXT_memory_budget` values.
- `create_pipeline_cache_safe(VkDevice, savedBytes, &cache)`: attempts to load saved pipeline-cache bytes and falls back to an empty cache if the driver rejects them.
- `export_pipeline_cache(VkDevice, cache, &bytes)`: checks Vulkan return codes and rejects empty or excessively large cache data.

## Integration notes

Include `native/vulkan_stability.hpp` from C++ code and link against the app's existing Vulkan loader/library. Call the device report after selecting a physical device. Use the cache helpers only with a valid device/cache and follow the renderer's synchronization and Android lifecycle rules.

This commit adds the helper layer and a compile-only CI check. It does **not** wire the helpers into the renderer, produce an APK, or demonstrate an FPS improvement. Runtime behavior must be validated on actual devices and drivers, including Mali GPUs.

## Limitations

- Heap size is not the same as currently available/free GPU memory.
- Memory-budget information is optional and depends on driver support.
- Pipeline cache data is implementation/driver-specific. Key stored caches by device and driver identity, and discard stale data after driver changes.
- Pipeline caches may reduce repeated compilation work, but do not guarantee hitch-free shader compilation.
- BC texture compression is reported as a capability only; this header does not decode unsupported texture formats.
