#include "Pawn.h"
#include "FrameRenderer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include <cassert>

Pawn::Pawn()
	: m_meshName()
	, m_textureName()
	, m_color( 0xFFFFFFFF )
	, m_meshPosition( 0.0f, 0.0f, 0.0f )
	, m_meshRotation( 0.0f, 0.0f, 0.0f )
	, m_meshScale( 1.0f, 1.0f, 1.0f )
{
}

Pawn::~Pawn()
{
	assert( !m_mesh );
	assert( !m_texture );
}

void Pawn::SetMesh( const std::string& meshName )
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

void Pawn::SetTexture( const std::string& textureName )
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
	return GetMeshLocalMatrix() * GetActorWorldMatrix();
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

void Pawn::OnRender( FrameRenderer& frame ) const
{
	Actor::OnRender( frame );

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
