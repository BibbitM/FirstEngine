#include "TextRenderer.h"
#include "Renderables.h"
#include <cassert>

TextRenderer::TextRenderer()
	: m_device( nullptr )
	, m_font( nullptr )
{
}

TextRenderer::~TextRenderer()
{
	assert( !m_device );
	assert( !m_font );
}

void TextRenderer::StartUp( IDirect3DDevice9* device )
{
	assert( device );
	assert( !m_device );
	assert( !m_font );

	m_device = device;

	D3DXCreateFont( m_device, 20, 0, FW_NORMAL, 1, false,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
					L"Arial", &m_font );
	assert( m_font );
}

void TextRenderer::ShutDown()
{
	assert( m_device );
	assert( m_font );

	if (m_font)
	{
		m_font->Release();
		m_font = 0;
	}

	m_device = nullptr;
}

void TextRenderer::Render( const std::vector< RenderableText >& texts )
{
	for( const auto& text : texts )
	{
		RECT textRect = { text.x, text.y, 0, 0 };

		m_font->DrawTextA( nullptr, text.text.c_str(), static_cast< INT >( text.text.length() ), &textRect, DT_CALCRECT, 0 );
		m_font->DrawTextA( nullptr, text.text.c_str(), static_cast< INT >( text.text.length() ), &textRect, DT_LEFT | DT_TOP, text.color );
	}
}
