#include "Device.hpp"

#include <Concerto/Core/Assert.hpp>
#include <Concerto/Core/Logger.hpp>

#include "PhysicalDevice.hpp"
#include "ShaderModule.hpp"

Device::Device(PhysicalDevice& physicalDevice)
{
	struct UserData {
		WGPUDevice* device;
		bool requestEnded;
	} userData = {
		.device = &_device,
		.requestEnded = false
	};

	WGPUDeviceDescriptor deviceDesc = {};
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "My Device"; // anything works here, that's your call
	deviceDesc.requiredFeaturesCount = 0; // we do not require any specific feature
	deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "The default queue";
	wgpuAdapterRequestDevice(physicalDevice.Get(), &deviceDesc, [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* pUserData)
		{
			UserData& userData = *static_cast<UserData*>(pUserData);
			if (status == WGPURequestDeviceStatus_Success)
				*userData.device = device;
			else Concerto::Logger::Error("Could not get WebGPU device: {}", message);
			userData.requestEnded = true;
		},
		&userData
	);

	wgpuDeviceSetUncapturedErrorCallback(_device, [](WGPUErrorType type, char const* message, void* /* pUserData */)
	{
		Concerto::Logger::Error("WebGPU error:\n\t {}", message);
	}, nullptr);


	CONCERTO_ASSERT(userData.requestEnded, "Need to wait");
}

Device::~Device()
{
	wgpuDeviceRelease(_device);
	_device = nullptr;
}

Queue Device::GetQueue()
{
	return Queue(wgpuDeviceGetQueue(_device));
}

SwapChain Device::CreateSwapChain(Concerto::UInt32 width, Concerto::UInt32 height, WGPUSurface surface, WGPUTextureFormat format)
{
	return {*this, width, height, surface, format};
}

RenderPipeline Device::CreateRenderPipeline(const ShaderModule& shaderModule, WGPUTextureFormat format)
{
	return {*this, shaderModule, format};
}

WGPUDevice Device::Get() const
{
	return _device;
}

ShaderModule Device::CreateShaderModule(const std::string& shaderSource)
{
	return {*this, shaderSource };
}
