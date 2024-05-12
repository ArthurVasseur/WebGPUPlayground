#include "CommandBuffer.hpp"

CommandBuffer::CommandBuffer(WGPUCommandBuffer commandBuffer) :
	_commandBuffer(commandBuffer)
{
}

CommandBuffer::~CommandBuffer()
{
	wgpuCommandBufferRelease(_commandBuffer);
}

WGPUCommandBuffer CommandBuffer::Get() const
{
	return _commandBuffer;
}
