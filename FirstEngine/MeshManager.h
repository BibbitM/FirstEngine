#pragma once

#include "NonCopyable.h"
#include <vector>

struct Mesh;
struct IDirect3DDevice9;

class MeshManager : public NonCopyable
{
public:
	MeshManager();
	~MeshManager();

	void StartUp( IDirect3DDevice9* device );
	void ShutDown();

	Mesh LoadMesh( const std::string& meshFile );
	void UnloadMesh( const Mesh& mesh );

private:
	IDirect3DDevice9* m_device;

	std::vector< std::pair< std::string, Mesh > > m_meshes;
};
