#pragma once
#include <webgpu/webgpu.h>

class CommandBuffer;
class Queue
{
public:
    Queue(WGPUQueue queue);
    ~Queue();
    WGPUQueue Get() const;
    void Submit(const CommandBuffer& commandBuffer);

private:
    WGPUQueue _queue;
};
