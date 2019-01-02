#pragma once

#include "NonCopyable.h"
#include "Mesh.h"

#include <memory>
#include <vector>

class FrameRenderer;
class Level;
class NavigationNode;
class Terrain;

class NavigationManager : public NonCopyable
{
public:
	NavigationManager();
	~NavigationManager();

	void StartUp( Level* level );
	void ShutDown();
	void Render( FrameRenderer& frame );

private:
	void CreateFromTerrain( const Terrain* terrain );
	void LoadResources();
	void UnloadResources();

	Level* m_level;

	std::vector< std::unique_ptr< NavigationNode > > m_nodesGrid;

	Mesh m_boxMesh;
	Mesh m_shpereMesh;
};
