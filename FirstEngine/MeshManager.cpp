#include "MeshManager.h"
#include "Mesh.h"
#include <d3dx9mesh.h>
#include <algorithm>
#include <cassert>

MeshManager::MeshManager()
	: m_device( nullptr )
{
}

MeshManager::~MeshManager()
{
	assert( !m_device );
}

void MeshManager::StartUp( IDirect3DDevice9* device )
{
	assert( device );
	assert( !m_device );
	assert( m_meshes.empty() );
	m_device = device;
}

void MeshManager::ShutDown()
{
	assert( m_device );
	m_device = nullptr;
	assert( m_meshes.empty() );
}

Mesh MeshManager::LoadMesh( const std::string& meshFile )
{
	auto foundIt = std::find_if( m_meshes.begin(), m_meshes.end(), [&meshFile]( std::pair< std::string, Mesh >& namedMesh ) { return namedMesh.first == meshFile; } );

	if( foundIt != m_meshes.end() )
	{
		foundIt->second.mesh->AddRef();
		return foundIt->second;
	}

	Mesh newMesh = {};
	D3DXLoadMeshFromXA( meshFile.c_str(), D3DXMESH_SYSTEMMEM, m_device, NULL, NULL, NULL, &newMesh.numMaterials, &newMesh.mesh );

	if( newMesh )
	{
		m_meshes.emplace_back( meshFile, newMesh );
		return newMesh;
	}

	assert( false );
	return Mesh{};
}

void MeshManager::UnloadMesh( const Mesh& mesh )
{
	auto foundIt = std::find_if( m_meshes.begin(), m_meshes.end(), [&mesh]( std::pair< std::string, Mesh >& namedMesh ) { return namedMesh.second.mesh == mesh.mesh; } );
	assert( foundIt != m_meshes.end() );

	if( foundIt != m_meshes.end() )
	{
		if( !foundIt->second.mesh->Release() )
		{
			if( foundIt != m_meshes.end() - 1 )
			{
				std::iter_swap( foundIt, m_meshes.end() - 1 );
			}
			m_meshes.pop_back();
		}
	}
}
