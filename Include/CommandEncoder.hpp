#pragma once
#include <string>
#include <memory>

#include <webgpu/webgpu.h>

#include "CommandBuffer.hpp"

class Device;
class TextureView;
class RenderPassEncoder;

class CommandEncoder
{
public:
	CommandEncoder(Device& device, const std::string& name);
	~CommandEncoder();

	void InsertDebugMarker(const std::string& label);
	CommandBuffer Finish();
	RenderPassEncoder* BeginRenderPass(const TextureView& textureView, const WGPUColor& clearColor = {});
	void EndRenderPass();

	WGPUCommandEncoder Get() const;
private:
	bool _released;
	WGPUCommandEncoder _commandEncoder;
	std::unique_ptr<RenderPassEncoder> _currentRenderPass;
};