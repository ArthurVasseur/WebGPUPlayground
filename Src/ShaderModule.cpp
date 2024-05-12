#include "ShaderModule.hpp"
#include "Device.hpp"

ShaderModule::ShaderModule(const Device& device, const std::string& wgslCode)
{
	WGPUShaderModuleDescriptor shaderDesc = {};
	shaderDesc.nextInChain = nullptr;
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;

	WGPUShaderModuleWGSLDescriptor shaderCodeDesc;
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
	shaderCodeDesc.code = wgslCode.c_str();
	shaderDesc.nextInChain = &shaderCodeDesc.chain;
	_shaderModule = wgpuDeviceCreateShaderModule(device.Get(), &shaderDesc);
}

ShaderModule::~ShaderModule()
{
	if (!_shaderModule)
		return;
	wgpuShaderModuleRelease(_shaderModule);
}

WGPUShaderModule ShaderModule::Get() const
{
	return _shaderModule;
}
