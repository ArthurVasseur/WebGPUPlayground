#include "RenderPassEncoder.hpp"

#include "RenderPipeline.hpp"

RenderPassEncoder::RenderPassEncoder(WGPURenderPassEncoder renderPassEncoder) :
	_renderPass(renderPassEncoder)
{
}

RenderPassEncoder::~RenderPassEncoder()
{
	wgpuRenderPassEncoderRelease(_renderPass);
}

void RenderPassEncoder::SetPipeline(const RenderPipeline& pipeline)
{
	wgpuRenderPassEncoderSetPipeline(_renderPass, pipeline.Get());
}

void RenderPassEncoder::DrawTriangle()
{
	wgpuRenderPassEncoderDraw(_renderPass, 3, 1, 0, 0);
}

WGPURenderPassEncoder RenderPassEncoder::Get() const
{
	return _renderPass;
}
