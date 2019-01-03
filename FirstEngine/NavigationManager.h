#pragma once

#include "NonCopyable.h"
#include "Mesh.h"

#include <memory>
#include <vector>

class FrameRenderer;
class Level;
class NavigationNode;
class Terrain;

struct D3DXVECTOR3;

class NavigationManager : public NonCopyable
{
public:
	NavigationManager();
	~NavigationManager();

	void StartUp( Level* level );
	void ShutDown();
	void Render( FrameRenderer& frame );

	std::vector< D3DXVECTOR3 > FindPath( const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	D3DXVECTOR3 FindPoint( const D3DXVECTOR3& position ) const;

private:
	const NavigationNode* FindNode( const D3DXVECTOR3& position ) const;
	std::vector< const NavigationNode* > Dijkstra( const NavigationNode* startNode, const NavigationNode* endNode ) const;

	void CreateFromTerrain( const Terrain* terrain );
	void LoadResources();
	void UnloadResources();

	int GetNodeIndex( const NavigationNode* node ) const;

	Level* m_level;

	std::vector< NavigationNode > m_nodesGrid;

	// DEBUG STUFF
	mutable std::vector< D3DXVECTOR3 > m_lastPath;
	// DEBUG END

	Mesh m_boxMesh;
	Mesh m_sphereMesh;

	static const float s_checkHeight;
	static const float s_maxHeightDiff;
};
