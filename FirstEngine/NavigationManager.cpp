#include "NavigationManager.h"
#include "NavigationNode.h"

#include "FrameRenderer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "MeshManager.h"
#include "Texture.h"
#include "Terrain.h"
#include <cassert>

NavigationManager::NavigationManager()
	: m_level( nullptr )
{
}

NavigationManager::~NavigationManager()
{
	assert( !m_level );
	assert( !m_boxMesh );
	assert( !m_shpereMesh );
}

void NavigationManager::StartUp( Level* level )
{
	assert( !m_level );

	m_level = level;
	LoadResources();
	CreateFromTerrain( m_level->GetTerrain() );
}

void NavigationManager::ShutDown()
{
	assert( m_level );

	m_nodesGrid.clear();
	UnloadResources();
	m_level = nullptr;
}

void NavigationManager::Render( FrameRenderer& frame )
{
	for( const auto& node : m_nodesGrid )
	{
		frame.AddMesh( Math::BuildMatrix( node->GetPosition() + D3DXVECTOR3( 0.0f, 0.5f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.1f, 1.0f, 0.1f) ), m_shpereMesh, Texture(), 0x00FF0000 );
	}
}

void NavigationManager::CreateFromTerrain( const Terrain* terrain )
{
	assert( m_nodesGrid.empty() );
	assert( terrain );

	m_nodesGrid.reserve( terrain->GetCount() * terrain->GetCount() );

	for( int x = 0; x < terrain->GetCount(); ++x )
	{
		for( int z = 0; z < terrain->GetCount(); ++z )
		{
			assert( m_nodesGrid.size() == terrain->GetIndex( x, z ) );

			m_nodesGrid.emplace_back( std::make_unique< NavigationNode >() );
			m_nodesGrid.back()->SetPosition( terrain->GetPosition( x, z ) );
		}
	}
}

void NavigationManager::LoadResources()
{
	assert( !m_boxMesh );
	assert( !m_shpereMesh );

	MeshManager* meshMgr = m_level->GetGame()->GetMeshManager();

	m_boxMesh = meshMgr->LoadMesh( "Content\\unitbox.x" );
	m_shpereMesh = meshMgr->LoadMesh( "Content\\unitsphere.x" );
}

void NavigationManager::UnloadResources()
{
	MeshManager* meshMgr = m_level->GetGame()->GetMeshManager();

	if( m_shpereMesh )
	{
		meshMgr->UnloadMesh( m_shpereMesh );
		m_shpereMesh.clear();
	}

	if( m_boxMesh )
	{
		meshMgr->UnloadMesh( m_boxMesh );
		m_boxMesh.clear();
	}
}
