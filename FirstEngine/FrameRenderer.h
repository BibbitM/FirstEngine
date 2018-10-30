#pragma once

#include "PlatformDefinitions.h"
#include <d3d9types.h>
#include <vector>
#include <string>

struct Mesh;
struct RenderableLine;
struct RenderableMesh;
struct RenderableText;
struct Texture;

struct D3DXCOLOR;
struct D3DXMATRIX;
struct D3DXVECTOR2;

class FrameRenderer
{
public:
	FrameRenderer();
	~FrameRenderer();

	void StartUp();
	void ShutDown();

	void Reset();

	void AddMesh( const D3DXMATRIX& matrix, const Mesh& mesh, const Texture& texture, const D3DXCOLOR& color );
	void AddLine( const D3DXVECTOR2& start, const D3DXVECTOR2& end, float width, const D3DXCOLOR& color );
	void AddText( int x, int y, const D3DXCOLOR& color, const char* formatText, ... );
	void AddText( int x, int y, const D3DXCOLOR& color, const std::string& text );

	const std::vector< RenderableMesh >& GetMeshes() const { return m_meshes; }
	const std::vector< RenderableLine >& GetLines() const { return m_lines; };
	const std::vector< RenderableText >& GetTexts() const { return m_texts; }

private:
	std::vector< RenderableMesh > m_meshes;
	std::vector< RenderableLine > m_lines;
	std::vector< RenderableText > m_texts;
};
