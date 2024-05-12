#include "Queue.hpp"
#include "CommandBuffer.hpp"

Queue::Queue(WGPUQueue queue) :
	_queue(queue)
{
}

Queue::~Queue()
{
	wgpuQueueRelease(_queue);
	_queue = nullptr;
}

WGPUQueue Queue::Get() const
{
	return _queue;
}

void Queue::Submit(const CommandBuffer& commandBuffer)
{
	const auto command = commandBuffer.Get();
	wgpuQueueSubmit(_queue, 1, &command);
}
