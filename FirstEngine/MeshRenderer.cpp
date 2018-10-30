#include "MeshRenderer.h"
#include "Renderables.h"
#include <cassert>

MeshRenderer::MeshRenderer()
	: m_device(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
	assert( !m_device );
}

void MeshRenderer::StartUp( IDirect3DDevice9* device )
{
	assert( device );
	assert( !m_device );
	m_device = device;
}

void MeshRenderer::ShutDown()
{
	assert( m_device );
	m_device = nullptr;
}

void MeshRenderer::Render( const std::vector< RenderableMesh >& meshes )
{
	for( const auto& mesh : meshes )
	{
		m_device->SetTransform( D3DTS_WORLD, &mesh.matrix );

		D3DMATERIAL9 mtrl = {};
		mtrl.Ambient = D3DXCOLOR( mesh.color );
		m_device->SetMaterial( &mtrl );

		m_device->SetTexture( 0, mesh.texture.texture );

		for( DWORD i = 0; i < mesh.mesh.numMaterials; ++i )
		{
			mesh.mesh.mesh->DrawSubset( i );
		}
	}
}
