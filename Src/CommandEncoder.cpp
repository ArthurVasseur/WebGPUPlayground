#include "CommandEncoder.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Device.hpp"
#include "TextureView.hpp"
#include "RenderPassEncoder.hpp"

CommandEncoder::CommandEncoder(Device& device, const std::string &name) :
	_released(false)
{
	WGPUCommandEncoderDescriptor encoderDesc = {};
	encoderDesc.nextInChain = nullptr;
	encoderDesc.label = name.data();
	_commandEncoder = wgpuDeviceCreateCommandEncoder(device.Get(), &encoderDesc);
}

CommandEncoder::~CommandEncoder()
{
	if (_released || !_commandEncoder)
		return;
	wgpuCommandEncoderRelease(_commandEncoder);

}

void CommandEncoder::InsertDebugMarker(const std::string &label)
{
	wgpuCommandEncoderInsertDebugMarker(_commandEncoder, label.c_str());
}

CommandBuffer CommandEncoder::Finish()
{
	WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
	cmdBufferDescriptor.nextInChain = nullptr;
	cmdBufferDescriptor.label = "Command buffer";
	CommandBuffer commandBuffer(wgpuCommandEncoderFinish(_commandEncoder, &cmdBufferDescriptor));
	_released = true;
	wgpuCommandEncoderRelease(_commandEncoder);
	_commandEncoder = nullptr;
	return commandBuffer;
}

RenderPassEncoder* CommandEncoder::BeginRenderPass(const TextureView& textureView, const WGPUColor& clearColor)
{
	if (_currentRenderPass)
	{
		CONCERTO_ASSERT_FALSE("Render pass not properly ended: Make sure to call CommandEncoder::EndRenderPass before CommandEncoder::BeginRenderPass.");
		return nullptr;
	}
	WGPURenderPassDescriptor renderPassDesc = {};
	WGPURenderPassColorAttachment renderPassColorAttachment = {};

	renderPassColorAttachment.view = textureView.Get();
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
	renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
	renderPassColorAttachment.clearValue = clearColor;
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;
	renderPassDesc.depthStencilAttachment = nullptr;
	renderPassDesc.timestampWriteCount = 0;
	renderPassDesc.timestampWrites = nullptr;
	renderPassDesc.nextInChain = nullptr;

	_currentRenderPass = std::make_unique<RenderPassEncoder>(wgpuCommandEncoderBeginRenderPass(_commandEncoder, &renderPassDesc));
	return _currentRenderPass.get();
}

void CommandEncoder::EndRenderPass()
{
	if (!_currentRenderPass || !_currentRenderPass->Get())
	{
		CONCERTO_ASSERT_FALSE("RenderPassEncoder not created: Ensure CommandEncoder::EndRenderPass is called only after creating a render pass.");
		return;
	}
	wgpuRenderPassEncoderEnd(_currentRenderPass->Get());
	_currentRenderPass.reset();
}

WGPUCommandEncoder CommandEncoder::Get() const
{
	return _commandEncoder;
}
