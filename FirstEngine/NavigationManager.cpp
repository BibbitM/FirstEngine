#include "NavigationManager.h"
#include "NavigationNode.h"

#include "CollisionResult.h"
#include "FrameRenderer.h"
#include "Game.h"
#include "Level.h"
#include "LevelCollisions.h"
#include "Math.h"
#include "MeshManager.h"
#include "Texture.h"
#include "Terrain.h"
#include <cassert>

const float NavigationManager::s_checkHeight = 1.0f;
const float NavigationManager::s_maxHeightDiff = 1.0f;

NavigationManager::NavigationManager()
	: m_level( nullptr )
{
}

NavigationManager::~NavigationManager()
{
	assert( !m_level );
	assert( !m_boxMesh );
	assert( !m_sphereMesh );
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
		// Render nodes.
		int color = node->GetNeightbours().empty() ? 0x00FF0000 : 0x0000FF00;
		frame.AddMesh( Math::BuildMatrix( node->GetPosition() + D3DXVECTOR3( 0.0f, 0.5f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.1f, 1.0f, 0.1f ) ), m_sphereMesh, Texture(), color );

		// Render connections.
		for( const auto& neightbour : node->GetNeightbours() )
		{
			D3DXVECTOR3 toNeightbour = neightbour->GetPosition() - node->GetPosition();
			D3DXVECTOR3 toNeightbourNorm;
			float toNeightbourLen = D3DXVec3Length( &toNeightbour );
			D3DXVec3Normalize( &toNeightbourNorm, &toNeightbour );
			float toNeightbourYaw = atan2f( toNeightbourNorm.x, toNeightbourNorm.z );
			float toNeightbourPitch = -asinf( toNeightbourNorm.y );

			D3DXMATRIX toNeighbourMatrix = Math::BuildMatrix( node->GetPosition() + Math::s_upVector3 * s_checkHeight + toNeightbour * 0.25f, D3DXVECTOR3( toNeightbourPitch, toNeightbourYaw, 0.0f ), D3DXVECTOR3( 0.01f, 0.01f, toNeightbourLen * 0.5f ) );
			frame.AddMesh( toNeighbourMatrix, m_boxMesh, Texture(), 0x000000FF );
		}
	}
}

void NavigationManager::CreateFromTerrain( const Terrain* terrain )
{
	assert( m_level );
	assert( m_nodesGrid.empty() );
	assert( terrain );

	m_nodesGrid.reserve( terrain->GetCount() * terrain->GetCount() );

	// Build nodes grid.
	for( int x = 0; x < terrain->GetCount(); ++x )
	{
		for( int z = 0; z < terrain->GetCount(); ++z )
		{
			assert( m_nodesGrid.size() == terrain->GetIndex( x, z ) );

			m_nodesGrid.emplace_back( std::make_unique< NavigationNode >() );
			m_nodesGrid.back()->SetPosition( terrain->GetPosition( x, z ) );
		}
	}

	// Find connection between nodes
	LevelCollisions levelCollisions( *m_level );

	for( int x = 0; x < terrain->GetCount(); ++x )
	{
		for( int z = 0; z < terrain->GetCount(); ++z )
		{
			NavigationNode* node = m_nodesGrid[ terrain->GetIndex( x, z ) ].get();

			for( int neighbourX = x - 1; neighbourX <= x + 1; ++neighbourX )
			{
				for( int neighbourZ = z - 1; neighbourZ <= z + 1; ++neighbourZ )
				{
					if( neighbourX == x && neighbourZ == z )
						continue;

					int neighbourIdx = terrain->GetIndex( neighbourX, neighbourZ );
					if( neighbourIdx < 0 )
						continue;

					NavigationNode* neighbourNode = m_nodesGrid[ neighbourIdx ].get();

					// Check height difference.
					if( fabsf( node->GetPosition().y - neighbourNode->GetPosition().y ) > s_maxHeightDiff )
						continue;

					// Check visibility.
					CollisionResult collision;
					D3DXVECTOR3 nodeCheckPosition = node->GetPosition() + Math::s_upVector3 * s_checkHeight;
					D3DXVECTOR3 neighbourNodeCheckPosition = neighbourNode->GetPosition() + Math::s_upVector3 * s_checkHeight;
					if( levelCollisions.GetLineTrace( collision, nodeCheckPosition, neighbourNodeCheckPosition ) || levelCollisions.GetLineTrace( collision, neighbourNodeCheckPosition, nodeCheckPosition ) )
						continue;

					node->AddNeigbour( neighbourNode );
				}
			}
		}
	}
}

void NavigationManager::LoadResources()
{
	assert( !m_boxMesh );
	assert( !m_sphereMesh );

	MeshManager* meshMgr = m_level->GetGame()->GetMeshManager();

	m_boxMesh = meshMgr->LoadMesh( "Content\\unitbox.x" );
	m_sphereMesh = meshMgr->LoadMesh( "Content\\unitsphere.x" );
}

void NavigationManager::UnloadResources()
{
	MeshManager* meshMgr = m_level->GetGame()->GetMeshManager();

	if( m_sphereMesh )
	{
		meshMgr->UnloadMesh( m_sphereMesh );
		m_sphereMesh.clear();
	}

	if( m_boxMesh )
	{
		meshMgr->UnloadMesh( m_boxMesh );
		m_boxMesh.clear();
	}
}
