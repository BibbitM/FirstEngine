#pragma once

#include "NonCopyable.h"
#include <vector>

struct IDirect3DDevice9;
struct ID3DXFont;
struct RenderableText;

class TextRenderer : public NonCopyable
{
public:
	TextRenderer();
	~TextRenderer();

	void StartUp( IDirect3DDevice9* device );
	void ShutDown();

	void Render( const std::vector< RenderableText >& texts );

private:
	IDirect3DDevice9* m_device;
	ID3DXFont* m_font;
};
