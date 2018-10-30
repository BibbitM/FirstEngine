#include "LineRenderer.h"
#include "Renderables.h"
#include <cassert>

LineRenderer::LineRenderer()
	: m_device( nullptr )
	, m_line( nullptr )
{
}

LineRenderer::~LineRenderer()
{
	assert( !m_device );
	assert( !m_line );
}

void LineRenderer::StartUp( IDirect3DDevice9* device )
{
	assert( device );
	assert( !m_device );
	assert( !m_line );

	m_device = device;

	D3DXCreateLine( m_device, &m_line );
	assert( m_line );
	m_line->SetAntialias( false );
	m_line->SetGLLines( true );
}

void LineRenderer::ShutDown()
{
	assert( m_device );
	assert( m_line );

	if( m_line )
	{
		m_line->Release();
		m_line = nullptr;
	}

	m_device = nullptr;
}

void LineRenderer::Render( const std::vector< RenderableLine >& lines )
{
	for( const auto& line : lines )
	{
		m_line->SetWidth( line.width );

		D3DXVECTOR2 vertices[] = { line.start, line.end };

		m_line->Begin();
		m_line->Draw( vertices, _countof( vertices ), line.color );
		m_line->End();
	}
}
