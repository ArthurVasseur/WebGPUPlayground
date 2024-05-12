#pragma once
#include <webgpu/webgpu.h>

class CommandBuffer
{
public:
	CommandBuffer(WGPUCommandBuffer commandBuffer);
	~CommandBuffer();
	WGPUCommandBuffer Get() const;
private:
	WGPUCommandBuffer _commandBuffer;
};