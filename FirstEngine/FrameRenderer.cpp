#include "FrameRenderer.h"
#include "Renderables.h"
#include <cassert>

FrameRenderer::FrameRenderer()
{
}

FrameRenderer::~FrameRenderer() = default;

void FrameRenderer::StartUp()
{
	// Do nothing.
}

void FrameRenderer::ShutDown()
{
	Reset();
}

void FrameRenderer::Reset()
{
	m_meshes.clear();
	m_lines.clear();
	m_texts.clear();
}

void FrameRenderer::AddMesh( const D3DXMATRIX& matrix, const Mesh& mesh, const Texture& texture, const D3DXCOLOR& color )
{
	assert( mesh.mesh );

	m_meshes.push_back( RenderableMesh{ matrix, mesh, texture, color } );
}

void FrameRenderer::AddLine( const D3DXVECTOR2& start, const D3DXVECTOR2& end, float width, const D3DXCOLOR& color )
{
	m_lines.push_back( RenderableLine{ start, end, width, color } );
}

void FrameRenderer::AddText( int x, int y, const D3DXCOLOR& color, const char* formatText, ... )
{
	va_list args;
	char buffer[ 1024 ];

	va_start( args, formatText );
	vsprintf_s( buffer, formatText, args );
	va_end( args );

	m_texts.push_back( RenderableText{ x, y, buffer, color } );
}

void FrameRenderer::AddText( int x, int y, const D3DXCOLOR& color, const std::string& text )
{
	m_texts.push_back( RenderableText{ x, y, text, color } );
}
