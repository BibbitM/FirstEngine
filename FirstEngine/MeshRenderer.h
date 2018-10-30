#pragma once

#include "NonCopyable.h"
#include <vector>

struct IDirect3DDevice9;
struct RenderableMesh;

class MeshRenderer : public NonCopyable
{
public:
	MeshRenderer();
	~MeshRenderer();

	void StartUp( IDirect3DDevice9* device );
	void ShutDown();

	void Render( const std::vector< RenderableMesh >& meshes );

private:
	IDirect3DDevice9* m_device;

};
