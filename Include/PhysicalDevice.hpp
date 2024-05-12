#pragma once
#include <webgpu.h>

class PhysicalDevice
{
public:
	PhysicalDevice(WGPUInstance instance, WGPUSurface surface, WGPUBackendType backend = WGPUBackendType_Vulkan, WGPUPowerPreference powerPreference = WGPUPowerPreference_HighPerformance);
	~PhysicalDevice();
	void PrintFeatures();

	WGPUAdapter Get() const;
private:
	WGPUInstance _instance;
	WGPUSurface _surface;
	WGPUAdapter _adapter;
};