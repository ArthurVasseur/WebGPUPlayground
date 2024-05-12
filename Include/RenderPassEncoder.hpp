#pragma once

#include <webgpu/webgpu.h>

class RenderPipeline;
class RenderPassEncoder
{
public:
    RenderPassEncoder(WGPURenderPassEncoder renderPassEncoder);
    ~RenderPassEncoder();

    void SetPipeline(const RenderPipeline& pipeline);
    void DrawTriangle();

    WGPURenderPassEncoder Get() const;

private:
    WGPURenderPassEncoder _renderPass;
};