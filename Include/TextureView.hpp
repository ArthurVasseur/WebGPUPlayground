#pragma once

#include <webgpu/webgpu.h>

class TextureView
{
public:
	TextureView(WGPUTextureView textureView);;
	~TextureView();
	WGPUTextureView Get() const;
private:
	WGPUTextureView _textureView;
};