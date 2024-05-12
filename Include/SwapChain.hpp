#pragma once
#include <Concerto/Core/Types.hpp>
#include <webgpu/webgpu.h>
#include "TextureView.hpp"

class Device;
class SwapChain
{
public:
	SwapChain(const Device& device, Concerto::UInt32 width, Concerto::UInt32 height, WGPUSurface surface, WGPUTextureFormat format);
	~SwapChain();

	void Present();
	TextureView GetCurrentTextureView() const;
private:
	WGPUSwapChain _swapChain;
};
