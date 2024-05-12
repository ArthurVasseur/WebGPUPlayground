#pragma once

#include <string>
#include <webgpu/webgpu.h>

class Device;
class ShaderModule
{
public:
    ShaderModule(const Device& device, const std::string& wgslCode);
    ~ShaderModule();

    constexpr const char* GetVertexEntryPointName() const { return "vs_main"; }
    constexpr const char* GetFragmentEntryPointName() const { return "fs_main"; }

    WGPUShaderModule Get() const;
private:
    WGPUShaderModule _shaderModule;
};