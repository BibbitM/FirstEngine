#pragma once

#include "NonCopyable.h"
#include <memory>
#include <vector>

class CameraManager;
class FrameRenderer;
class Game;
class Object;
class Terrain;

class Level : private NonCopyable
{
public:
	Level();
	~Level();

	void StartUp( Game* game );
	void ShutDown();

	void Update( float deltaTime );
	void Render( FrameRenderer& frame ) const;


	void RegisterObject( Object* object );
	void UnregisterObject( Object* object );

	template< class TObject >
	TObject* CreateObject();
	template< class TObject, typename TInitFunc >
	TObject* CreateObject( TInitFunc initFunc );
	void DestroyObject( Object* object );


	Game* GetGame() const;
	CameraManager* GetCameraManager() const;

	Terrain* GetTerrain() const;

private:
	void LoadTerrain();

	Game* m_game;

	std::unique_ptr< CameraManager > m_cameraManager;

	std::vector< Object* > m_objects;
	std::vector< Object* > m_registeredObjects;

	// Game specific objects;
	Terrain* m_terrain;
};

template< class TObject >
TObject* Level::CreateObject()
{
	TObject* object = new TObject();
	object->StartUp( this );
	return object;
}

template< class TObject, typename TInitFunc >
TObject* Level::CreateObject( TInitFunc initFunc )
{
	TObject* object = new TObject();
	initFunc( object );
	object->StartUp( this );
	return object;
}
