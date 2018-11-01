#include "Pawn.h"
#include "FrameRenderer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include <cassert>

Pawn::Pawn( const std::string& meshName, const std::string& textureName, const D3DXCOLOR& color, const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale )
	: m_meshName( meshName )
	, m_textureName( textureName )
	, m_color( color )
	, m_meshPosition( position )
	, m_meshRotation( rotation )
	, m_meshScale( scale )
{
}

Pawn::~Pawn()
{
	assert( !m_mesh );
	assert( !m_texture );
}

void Pawn::SetMeshName( const std::string& meshName )
{
	if( m_meshName == meshName )
	{
		return;
	}

	if( IsInitialized() )
	{
		UnloadMesh();
	}


	m_meshName = meshName;


	if( IsInitialized() )
	{
		LoadMesh();
	}
}

void Pawn::SetTextureName( const std::string& textureName )
{
	if( m_textureName == textureName )
	{
		return;
	}

	if( IsInitialized() )
	{
		UnloadTexture();
	}


	m_textureName = textureName;


	if( IsInitialized() )
	{
		LoadTexture();
	}
}

D3DXMATRIX Pawn::GetMeshLocalMatrix() const
{
	return Math::BuildMatrix( m_meshPosition, m_meshRotation, m_meshScale );
}

D3DXMATRIX Pawn::GetMeshWorldMatrix() const
{
	return GetActorWorldMatrix() * GetMeshLocalMatrix();
}

void Pawn::OnStartUp()
{
	Actor::OnStartUp();

	LoadMesh();
	LoadTexture();
}

void Pawn::OnShutDown()
{
	UnloadTexture();
	UnloadMesh();

	Actor::OnShutDown();
}

void Pawn::Render( FrameRenderer& frame ) const
{
	Actor::Render( frame );

	if( m_mesh )
	{
		frame.AddMesh( GetMeshWorldMatrix(), m_mesh, m_texture, m_color );
	}
}

void Pawn::LoadMesh()
{
	assert( !m_mesh );
	assert( IsInitialized() );

	if( !m_meshName.empty() )
	{
		m_mesh = GetLevel()->GetGame()->GetMeshManager()->LoadMesh( m_meshName );
	}
}

void Pawn::LoadTexture()
{
	assert( !m_texture );
	assert( IsInitialized() );

	if( !m_textureName.empty() )
	{
		m_texture = GetLevel()->GetGame()->GetTextureManager()->LoadTexture( m_textureName );
	}
}

void Pawn::UnloadMesh()
{
	assert( IsInitialized() );

	if( m_mesh )
	{
		GetLevel()->GetGame()->GetMeshManager()->UnloadMesh( m_mesh );
		m_mesh.clear();
	}
}

void Pawn::UnloadTexture()
{
	assert( IsInitialized() );

	if( m_texture )
	{

		GetLevel()->GetGame()->GetTextureManager()->UnloadTexture( m_texture );
		m_texture.clear();
	}
}
