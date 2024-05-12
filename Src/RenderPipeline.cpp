#include "RenderPipeline.hpp"
#include "Device.hpp"
#include "ShaderModule.hpp"

RenderPipeline::RenderPipeline(const Device& device, const ShaderModule& shaderModule, WGPUTextureFormat format)
{
	WGPURenderPipelineDescriptor pipelineDesc = {};
	pipelineDesc.nextInChain = nullptr;

	pipelineDesc.vertex.bufferCount = 0;
	pipelineDesc.vertex.buffers = nullptr;
	pipelineDesc.vertex.module = shaderModule.Get();
	pipelineDesc.vertex.entryPoint = shaderModule.GetVertexEntryPointName();
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
	pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
	pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
	pipelineDesc.primitive.cullMode = WGPUCullMode_None;

	WGPUFragmentState fragmentState = {};
	fragmentState.nextInChain = nullptr;
	fragmentState.module = shaderModule.Get();
	fragmentState.entryPoint = shaderModule.GetFragmentEntryPointName();
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;

	pipelineDesc.fragment = &fragmentState;

	WGPUBlendState blendState;
	// Usual alpha blending for the color:
	blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	blendState.color.operation = WGPUBlendOperation_Add;
	// We leave the target alpha untouched:
	blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	blendState.alpha.dstFactor = WGPUBlendFactor_One;
	blendState.alpha.operation = WGPUBlendOperation_Add;

	WGPUColorTargetState colorTarget = {};
	colorTarget.nextInChain = nullptr;
	colorTarget.format = format;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = WGPUColorWriteMask_All;

	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;

	pipelineDesc.depthStencil = nullptr;

	pipelineDesc.multisample.count = 1;
	pipelineDesc.multisample.mask = ~0u;
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	pipelineDesc.layout = nullptr;

	_renderPipeline =  wgpuDeviceCreateRenderPipeline(device.Get(), &pipelineDesc);
}

RenderPipeline::~RenderPipeline()
{
	if (!_renderPipeline)
		return;
	wgpuRenderPipelineRelease(_renderPipeline);
}

WGPURenderPipeline RenderPipeline::Get() const
{
	return _renderPipeline;
}
