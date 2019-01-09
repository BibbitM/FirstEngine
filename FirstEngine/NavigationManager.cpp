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

#include <algorithm>
#include <cassert>

const float NavigationManager::s_checkHeight = 0.5f;
const float NavigationManager::s_maxHeightDiff = 0.5f;

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
		int color = node.GetNeightbours().empty() ? 0x00FF0000 : 0x0000FF00;
		frame.AddMesh( Math::BuildMatrix( node.GetPosition() + D3DXVECTOR3( 0.0f, 0.5f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.1f, 1.0f, 0.1f ) ), m_sphereMesh, Texture(), color );

		// Render connections.
		for( const auto& neightbour : node.GetNeightbours() )
		{
			D3DXVECTOR3 toNeightbour = neightbour->GetPosition() - node.GetPosition();
			D3DXVECTOR3 toNeightbourNorm;
			float toNeightbourLen = D3DXVec3Length( &toNeightbour );
			D3DXVec3Normalize( &toNeightbourNorm, &toNeightbour );
			float toNeightbourYaw = atan2f( toNeightbourNorm.x, toNeightbourNorm.z );
			float toNeightbourPitch = -asinf( toNeightbourNorm.y );

			D3DXMATRIX toNeighbourMatrix = Math::BuildMatrix( node.GetPosition() + Math::s_upVector3 * s_checkHeight + toNeightbour * 0.25f, D3DXVECTOR3( toNeightbourPitch, toNeightbourYaw, 0.0f ), D3DXVECTOR3( 0.01f, 0.01f, toNeightbourLen * 0.5f ) );
			frame.AddMesh( toNeighbourMatrix, m_boxMesh, Texture(), 0x000000FF );
		}
	}

	for( int i = 0; i < m_lastPath.size(); ++i )
	{
		// Render points.
		int color = 0x00FFFF00;
		if( i == 0 )
			color = 0x0000FFFF;
		else if( i == m_lastPath.size() - 1 )
			color = 0x00FF00FF;

		const D3DXVECTOR3& position = m_lastPath[ i ];

		frame.AddMesh( Math::BuildMatrix( position + D3DXVECTOR3( 0.0f, 0.5f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.2f, 1.0f, 0.2f ) ), m_sphereMesh, Texture(), color );

		// Render connection.
		if( i > 0 )
		{
			const D3DXVECTOR3& prevPosition = m_lastPath[ i - 1 ];

			D3DXVECTOR3 section = position - prevPosition;
			D3DXVECTOR3 sectionNorm;
			float sectionLen = D3DXVec3Length( &section );
			D3DXVec3Normalize( &sectionNorm, &section );
			float sectionYaw = atan2f( sectionNorm.x, sectionNorm.z );
			float sectionPitch = -asinf( sectionNorm.y );

			D3DXMATRIX toNeighbourMatrix = Math::BuildMatrix( prevPosition + Math::s_upVector3 * s_checkHeight + section * 0.5f, D3DXVECTOR3( sectionPitch, sectionYaw, 0.0f ), D3DXVECTOR3( 0.05f, 0.05f, sectionLen ) );
			frame.AddMesh( toNeighbourMatrix, m_boxMesh, Texture(), 0x00CCCC0C );
		}
	}
}

std::vector< D3DXVECTOR3 > NavigationManager::FindPath( const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	std::vector< D3DXVECTOR3 > path;

	auto startNode = FindNode( start );
	auto endNode = FindNode( end );

	if( startNode && endNode )
	{
		std::vector< const NavigationNode* > nodes = Dijkstra( startNode, endNode );

		if( !nodes.empty() )
		{
			path.reserve( nodes.size() + 2 );

			path.push_back( start );
			for( auto node : nodes )
			{
				path.push_back( node->GetPosition() );
			}
			path.push_back( end );
		}
	}

	// DEBUG STUFF
	if( path.empty() )
	{
		m_lastPath.clear();
		m_lastPath.push_back( start );
	}
	else
	{
		m_lastPath = path;
	}
	// DEBUG END

	return path;
}

D3DXVECTOR3 NavigationManager::FindPoint( const D3DXVECTOR3& position ) const
{
	auto node = FindNode( position );

	return node ? node->GetPosition() : position;
}

const NavigationNode* NavigationManager::FindNode( const D3DXVECTOR3& position ) const
{
	assert( m_level );

	Terrain* terrain = m_level->GetTerrain();

	assert( terrain );

	int idx = terrain->GetIndex( position.x, position.z );

	return ( idx >= 0 ) ? &m_nodesGrid[ idx ] : nullptr;
}

std::vector< const NavigationNode* > NavigationManager::Dijkstra( const NavigationNode* startNode, const NavigationNode* endNode ) const
{
	assert( startNode );
	assert( endNode );

	std::vector< const NavigationNode* > nodes;

	if( startNode == endNode )
	{
		nodes.push_back( startNode );
	}
	else
	{
		std::vector< int > vertexSet;
		std::vector< float > distances;
		std::vector< int > previous;

		auto sortSet = [ &distances ]( int left, int right )
		{
			return distances[ left ] > distances[ right ];
		};


		vertexSet.resize( m_nodesGrid.size() );
		for( int i = 0; i < vertexSet.size(); ++i )
			vertexSet[ i ] = i;

		distances.resize( m_nodesGrid.size(), FLT_MAX );
		previous.resize( m_nodesGrid.size(), -1 );

		distances[ GetNodeIndex( startNode ) ] = 0.0f;

		while( !vertexSet.empty() )
		{
			std::sort( vertexSet.begin(), vertexSet.end(), sortSet );

			int nodeIdx = vertexSet.back();
			vertexSet.pop_back();

			const NavigationNode& node = m_nodesGrid[ nodeIdx ];
			for( const NavigationNode* neighbour : node.GetNeightbours() )
			{
				int neightbourIdx = GetNodeIndex( neighbour );

				float alternate = distances[ nodeIdx ] + CalculateDistance( node, *neighbour );
				if( alternate < distances[ neightbourIdx ] )
				{
					distances[ neightbourIdx ] = alternate;
					previous[ neightbourIdx ] = nodeIdx;
				}
			}
		}


		int endIdx = GetNodeIndex( endNode );
		if( previous[ endIdx ] != -1 )
		{
			int prevIdx = previous[ endIdx ];

			while( prevIdx != -1 )
			{
				nodes.push_back( &m_nodesGrid[ prevIdx ] );
				prevIdx = previous[ prevIdx ];
			}
			std::reverse( nodes.begin(), nodes.end() );
		}

	}

	return nodes;
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

			m_nodesGrid.emplace_back();
			m_nodesGrid.back().SetPosition( terrain->GetPosition( x, z ) );
		}
	}

	// Find connection between nodes
	LevelCollisions levelCollisions( *m_level );

	for( int x = 0; x < terrain->GetCount(); ++x )
	{
		for( int z = 0; z < terrain->GetCount(); ++z )
		{
			NavigationNode& node = m_nodesGrid[ terrain->GetIndex( x, z ) ];

			for( int neighbourX = x - 1; neighbourX <= x + 1; ++neighbourX )
			{
				for( int neighbourZ = z - 1; neighbourZ <= z + 1; ++neighbourZ )
				{
					if( neighbourX == x && neighbourZ == z )
						continue;

					int neighbourIdx = terrain->GetIndex( neighbourX, neighbourZ );
					if( neighbourIdx < 0 )
						continue;

					NavigationNode& neighbourNode = m_nodesGrid[ neighbourIdx ];

					// Check height difference.
					if( fabsf( node.GetPosition().y - neighbourNode.GetPosition().y ) > s_maxHeightDiff )
						continue;

					// Check visibility.
					CollisionResult collision;
					D3DXVECTOR3 nodeCheckPosition = node.GetPosition() + Math::s_upVector3 * s_checkHeight;
					D3DXVECTOR3 neighbourNodeCheckPosition = neighbourNode.GetPosition() + Math::s_upVector3 * s_checkHeight;
					if( levelCollisions.GetLineTrace( collision, nodeCheckPosition, neighbourNodeCheckPosition ) || levelCollisions.GetLineTrace( collision, neighbourNodeCheckPosition, nodeCheckPosition ) )
						continue;

					node.AddNeigbour( neighbourNode );
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

int NavigationManager::GetNodeIndex( const NavigationNode* node ) const
{
	return ( int )std::distance( &m_nodesGrid.front(), node );
}
