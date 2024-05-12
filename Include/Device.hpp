#pragma once
#include <string>

#include <Concerto/Core/Types.hpp>
#include <webgpu/webgpu.h>

#include "Queue.hpp"
#include "SwapChain.hpp"
#include "RenderPipeline.hpp"

class PhysicalDevice;
class ShaderModule;
class Device
{
public:
	Device(PhysicalDevice& physicalDevice);
	~Device();

	Queue GetQueue();
	SwapChain CreateSwapChain(Concerto::UInt32 width, Concerto::UInt32 height, WGPUSurface surface, WGPUTextureFormat format = WGPUTextureFormat_RGBA8Unorm);
	RenderPipeline CreateRenderPipeline(const ShaderModule& shaderModule, WGPUTextureFormat format);

	WGPUDevice Get() const;
	ShaderModule CreateShaderModule(const std::string& shaderSource);

private:
	WGPUDevice _device;
};
