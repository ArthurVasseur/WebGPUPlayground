#include "TextureView.hpp"

TextureView::TextureView(WGPUTextureView textureView) :
	_textureView(textureView)
{
}

TextureView::~TextureView()
{
	if (!_textureView)
		return;
	wgpuTextureViewRelease(_textureView);
}

WGPUTextureView TextureView::Get() const
{
	return _textureView;
}
