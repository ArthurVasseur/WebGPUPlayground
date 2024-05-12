#include "SwapChain.hpp"

#include "Device.hpp"

SwapChain::SwapChain(const Device& device, Concerto::UInt32 width, Concerto::UInt32 height, WGPUSurface surface, WGPUTextureFormat format)
{
	WGPUSwapChainDescriptor swapChainDesc = {};
	swapChainDesc.nextInChain = nullptr;
	swapChainDesc.width = width;
	swapChainDesc.height = height;
	swapChainDesc.format = format;
	swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;
	swapChainDesc.presentMode = WGPUPresentMode_Fifo;
	_swapChain =  wgpuDeviceCreateSwapChain(device.Get(), surface, &swapChainDesc);
}

SwapChain::~SwapChain()
{
	wgpuSwapChainRelease(_swapChain);
}

void SwapChain::Present()
{
	wgpuSwapChainPresent(_swapChain);
}

TextureView SwapChain::GetCurrentTextureView() const
{
	return { wgpuSwapChainGetCurrentTextureView(_swapChain)};
}
