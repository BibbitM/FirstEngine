#include "Terrain.h"
#include "FrameRenderer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "MeshManager.h"
#include "ShapeAabb.h"
#include "TextureManager.h"
#include <cassert>
#include <d3dx9math.h>
#include <fstream>

const std::string Terrain::s_boxMeshName = "Content\\unitbox.x";
const std::string Terrain::s_grassTextureName = "Content\\GrassGreenTexture0002.jpg";
const std::string Terrain::s_lavaTextureName = "Content\\lava.jpg";

Terrain::Terrain()
	: m_size( 0.0f )
	, m_height( 0.0f )
	, m_count( 0 )
{
}

Terrain::~Terrain()
{
	assert( !m_boxMesh );
	assert( !m_grassTexture );
	assert( !m_lavaTexture );
}

void Terrain::SetTerrainFile( const std::string& terrainFile )
{
	assert( !IsInitialized() && "Terrain file must be set before initialization." );

	m_terrainFile = terrainFile;
}

float Terrain::GetHeight( int index ) const
{
	if( index < 0 || index >= m_count * m_count )
	{
		return 0.0f;
	}

	return m_tiles[ index ] * m_height;
}

float Terrain::GetHeight( float x, float z ) const
{
	return GetHeight( GetIndex( x, z ) );
}

float Terrain::GetHeight( const D3DXVECTOR3& pos ) const
{
	return GetHeight( pos.x, pos.z );
}

ShapeAabb Terrain::GetCollisionShape( int x, int z ) const
{
	const int index = GetIndex( x, z );

	if( index < 0 )
	{
		return ShapeAabb();
	}

	ShapeAabb shape;
	shape.m_origin = GetPosition( x, z );
	shape.m_halfSize = D3DXVECTOR3( m_size * 0.5f, m_height * 0.5f, m_size * 0.5f );

	return shape;
}

void Terrain::OnStartUp()
{
	Object::OnStartUp();

	LoadResources();

	LoadFromFile();
}

void Terrain::OnShutDown()
{
	UnloadResources();

	Object::OnShutDown();
}

void Terrain::OnRender( FrameRenderer& frame ) const
{
	for( int x = 0; x < m_count; ++x )
	{
		for( int z = 0; z < m_count; ++z )
		{
			const int index = GetIndex( x, z );
			float height = GetHeight( index );
			D3DXVECTOR3 position = GetPosition( x, z );

			if( height <= 0 )
			{
				D3DXVECTOR3 center = position;
				center.y -= 0.5f;

				D3DXMATRIX matLava = Math::BuildMatrix( center, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( m_size, 1.0f, m_size ) );
				frame.AddMesh( matLava, m_boxMesh, m_lavaTexture, 0xFFFFFFFF );
			}
			else
			{
				D3DXVECTOR3 center = position;
				center.y += height * 0.5f;

				D3DXMATRIX matBox = Math::BuildMatrix( center, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( m_size, height, m_size ) );
				frame.AddMesh( matBox, m_boxMesh, m_grassTexture, GetColorForHeight( height ) );
			}
		}
	}
}

void Terrain::LoadFromFile()
{
	assert( m_tiles.empty() );

	std::ifstream file( m_terrainFile );
	assert( file.is_open() );

	file >> m_size;
	file.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	assert( m_size > 0.0f );

	file >> m_height;
	file.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	assert( m_height > 0.0f );

	file >> m_count;
	file.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	assert( m_count > 0 );

	if( m_count > 0 )
	{
		m_tiles.resize( m_count * m_count, 0 );
	}

	for( int x = 0; x < m_count; ++x )
	{
		for( int z = 0; z < m_count; ++z )
		{
			file >> m_tiles[ GetIndex( x, z ) ];
			assert( m_tiles[ GetIndex( x, z ) ] >= 0 );
		}
		file.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	}
}

void Terrain::LoadResources()
{
	assert( !m_boxMesh );
	assert( !m_grassTexture );
	assert( !m_lavaTexture );
	assert( IsInitialized() );

	MeshManager* meshMgr = GetLevel()->GetGame()->GetMeshManager();
	TextureManager* textureMgr = GetLevel()->GetGame()->GetTextureManager();

	m_boxMesh = meshMgr->LoadMesh( s_boxMeshName );

	m_grassTexture = textureMgr->LoadTexture( s_grassTextureName );
	m_lavaTexture = textureMgr->LoadTexture( s_lavaTextureName );
}

void Terrain::UnloadResources()
{
	assert( IsInitialized() );

	MeshManager* meshMgr = GetLevel()->GetGame()->GetMeshManager();
	TextureManager* textureMgr = GetLevel()->GetGame()->GetTextureManager();

	if( m_lavaTexture )
	{
		textureMgr->UnloadTexture( m_lavaTexture );
		m_lavaTexture.clear();
	}

	if( m_grassTexture )
	{
		textureMgr->UnloadTexture( m_grassTexture );
		m_grassTexture.clear();
	}

	if( m_boxMesh )
	{
		meshMgr->UnloadMesh( m_boxMesh );
		m_boxMesh.clear();
	}
}

int Terrain::GetIndex( float x, float z ) const
{
	D3DXVECTOR3 corner = GetCorner();

	x -= corner.x;
	z -= corner.z;

	return GetIndex( static_cast< int >( floor( x / m_size ) ), static_cast< int >( floor( z / m_size ) ) );
}

int Terrain::GetIndex( int x, int z ) const
{
	if( x < 0 || x >= m_count )
	{
		return -1;
	}

	if( z < 0 || z >= m_count )
	{
		return -1;
	}

	return x * m_count + z;
}

D3DXVECTOR3 Terrain::GetCorner() const
{
	return D3DXVECTOR3( -( m_size * m_count * 0.5f ), 0.0f, -( m_size * m_count * 0.5f ) );
}

D3DXVECTOR3 Terrain::GetPosition( int x, int z ) const
{
	assert( x >= 0 && x < m_count );
	assert( z >= 0 && z < m_count );

	return GetCorner() + D3DXVECTOR3( m_size * ( x + 0.5f ), 0.0f, m_size * ( z + 0.5f ) );
}

D3DXCOLOR Terrain::GetColorForHeight( float height )
{
	return Math::Lerp( D3DXCOLOR( 0xFFFF2222 ), D3DXCOLOR( 0xFFFFFFFF ), Math::Clamp( Math::Square( height * 0.5f ), 0.0f, 1.0f ) );
}
