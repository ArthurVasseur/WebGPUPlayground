#include "PhysicalDevice.hpp"

#include <Concerto/Core/Assert.hpp>
#include <Concerto/Core/Logger.hpp>
#include <vector>

PhysicalDevice::PhysicalDevice(WGPUInstance instance, WGPUSurface surface, WGPUBackendType backend, WGPUPowerPreference powerPreference) :
	_instance(instance),
	_surface(surface)
{
	struct UserData
	{
		WGPUAdapter* adapter;
		bool requestEnded;
	} userData = {
		.adapter = &_adapter,
		.requestEnded = false
	};

	WGPURequestAdapterOptions options = {};
	options.nextInChain = nullptr;
	options.powerPreference = powerPreference;
	options.backendType = backend;
	options.compatibleSurface = _surface;

	wgpuInstanceRequestAdapter(instance, &options, [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData)
		{
			UserData& userData = *static_cast<UserData*>(pUserData);
			if (status == WGPURequestAdapterStatus_Success)
				*userData.adapter = adapter;
			else Concerto::Logger::Error("Could not get WebGPU adapter: {}", message);
			userData.requestEnded = true;
		}, &userData);

	CONCERTO_ASSERT(userData.requestEnded == true, "Need to wait");
}

PhysicalDevice::~PhysicalDevice()
{
	wgpuAdapterRelease(_adapter);
	_adapter = nullptr;
}

void PhysicalDevice::PrintFeatures()
{
	std::vector<WGPUFeatureName> features;
	const std::size_t featureCount = wgpuAdapterEnumerateFeatures(_adapter, nullptr);
	features.resize(featureCount);
	wgpuAdapterEnumerateFeatures(_adapter, features.data());

	WGPUSupportedLimits limits = {};
	limits.nextInChain = nullptr;
	const bool success = wgpuAdapterGetLimits(_adapter, &limits);
	if (!success)
	{
		CONCERTO_ASSERT_FALSE("Could not retreive physical device limits");
		return;
	}
	Concerto::Logger::Info("Adapter limits:");
	Concerto::Logger::Info(" - maxTextureDimension1D: {}", limits.limits.maxTextureDimension1D);
	Concerto::Logger::Info(" - maxTextureDimension2D: {}", limits.limits.maxTextureDimension2D);
	Concerto::Logger::Info(" - maxTextureDimension3D: {}", limits.limits.maxTextureDimension3D);
	Concerto::Logger::Info(" - maxTextureArrayLayers: {}", limits.limits.maxTextureArrayLayers);
	Concerto::Logger::Info(" - maxBindGroups: {}", limits.limits.maxBindGroups);
	Concerto::Logger::Info(" - maxDynamicUniformBuffersPerPipelineLayout: {}", limits.limits.maxDynamicUniformBuffersPerPipelineLayout);
	Concerto::Logger::Info(" - maxDynamicStorageBuffersPerPipelineLayout: {}", limits.limits.maxDynamicStorageBuffersPerPipelineLayout);
	Concerto::Logger::Info(" - maxSampledTexturesPerShaderStage: {}", limits.limits.maxSampledTexturesPerShaderStage);
	Concerto::Logger::Info(" - maxSamplersPerShaderStage: {}", limits.limits.maxSamplersPerShaderStage);
	Concerto::Logger::Info(" - maxStorageBuffersPerShaderStage: {}", limits.limits.maxStorageBuffersPerShaderStage);
	Concerto::Logger::Info(" - maxStorageTexturesPerShaderStage: {}", limits.limits.maxStorageTexturesPerShaderStage);
	Concerto::Logger::Info(" - maxUniformBuffersPerShaderStage: {}", limits.limits.maxUniformBuffersPerShaderStage);
	Concerto::Logger::Info(" - maxUniformBufferBindingSize: {}", limits.limits.maxUniformBufferBindingSize);
	Concerto::Logger::Info(" - maxStorageBufferBindingSize: {}", limits.limits.maxStorageBufferBindingSize);
	Concerto::Logger::Info(" - minUniformBufferOffsetAlignment: {}", limits.limits.minUniformBufferOffsetAlignment);
	Concerto::Logger::Info(" - minStorageBufferOffsetAlignment: {}", limits.limits.minStorageBufferOffsetAlignment);
	Concerto::Logger::Info(" - maxVertexBuffers: {}", limits.limits.maxVertexBuffers);
	Concerto::Logger::Info(" - maxVertexAttributes: {}", limits.limits.maxVertexAttributes);
	Concerto::Logger::Info(" - maxVertexBufferArrayStride: {}", limits.limits.maxVertexBufferArrayStride);
	Concerto::Logger::Info(" - maxInterStageShaderComponents: {}", limits.limits.maxInterStageShaderComponents);
	Concerto::Logger::Info(" - maxComputeWorkgroupStorageSize: {}", limits.limits.maxComputeWorkgroupStorageSize);
	Concerto::Logger::Info(" - maxComputeInvocationsPerWorkgroup: {}", limits.limits.maxComputeInvocationsPerWorkgroup);
	Concerto::Logger::Info(" - maxComputeWorkgroupSizeX: {}", limits.limits.maxComputeWorkgroupSizeX);
	Concerto::Logger::Info(" - maxComputeWorkgroupSizeY: {}", limits.limits.maxComputeWorkgroupSizeY);
	Concerto::Logger::Info(" - maxComputeWorkgroupSizeZ: {}", limits.limits.maxComputeWorkgroupSizeZ);
	Concerto::Logger::Info(" - maxComputeWorkgroupsPerDimension: {}", limits.limits.maxComputeWorkgroupsPerDimension);

	WGPUAdapterProperties properties = {};
	properties.nextInChain = nullptr;
	wgpuAdapterGetProperties(_adapter, &properties);
	Concerto::Logger::Info("Adapter properties:");
	Concerto::Logger::Info(" - vendorID: {}", properties.vendorID);
	Concerto::Logger::Info(" - deviceID: {}", properties.deviceID);
	Concerto::Logger::Info(" - name: {}", properties.name);
	if (properties.driverDescription)
		Concerto::Logger::Info(" - driverDescription: {}", properties.driverDescription);
	Concerto::Logger::Info(" - adapterType: {}", static_cast<Concerto::Int32>(properties.adapterType));
	Concerto::Logger::Info(" - backendType: {}", static_cast<Concerto::Int32>(properties.backendType));
}

WGPUAdapter PhysicalDevice::Get() const
{
	return _adapter;
}
