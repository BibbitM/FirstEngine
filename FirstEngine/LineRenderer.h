#pragma once

#include "NonCopyable.h"
#include <vector>

struct IDirect3DDevice9;
struct ID3DXLine;
struct RenderableLine;

class LineRenderer : public NonCopyable
{
public:
	LineRenderer();
	~LineRenderer();

	void StartUp( IDirect3DDevice9* device );
	void ShutDown();

	void Render( const std::vector< RenderableLine >& lines );

private:
	IDirect3DDevice9* m_device;
	ID3DXLine* m_line;
};
