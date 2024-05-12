#pragma once

#include <webgpu/webgpu.h>

class Device;
class ShaderModule;
class RenderPipeline
{
public:
    RenderPipeline(const Device& device, const ShaderModule& shaderModule, WGPUTextureFormat format);
    ~RenderPipeline();
    WGPURenderPipeline Get() const;
private:
    WGPURenderPipeline _renderPipeline;
};