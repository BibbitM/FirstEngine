#include "Level.h"
#include "CameraManager.h"
#include "GameMode.h"
#include "NavigationManager.h"
#include "Object.h"
#include "Terrain.h"
#include <algorithm>
#include <cassert>

// TEMP STUFF
#include "Car.h"
#include "Game.h"
#include "FreeCamera.h"
#include "InputManager.h"
#include "PacMan.h"
#include "TestCameraPoint.h"
#include "TestCollisionShape.h"
#include "Tiger.h"
// TEMP END

namespace
{
	bool s_debugNavigationManager = false;
}

Level::Level()
	: m_game( nullptr )
	, m_cameraManager( std::make_unique< CameraManager >() )
	, m_navigationManager( std::make_unique< NavigationManager >() )
	, m_terrain( nullptr )
	, m_gameMode( nullptr )
{
}

Level::~Level()
{
	assert( !m_game );
	assert( m_objects.empty() );
	assert( m_registeredObjects.empty() );
	assert( !m_terrain );
	assert( !m_gameMode );
}

void Level::StartUp( Game* game )
{
	assert( game );
	assert( !m_game );
	m_game = game;

	m_cameraManager->StartUp( this );

	LoadTerrain();
	m_gameMode = CreateObject< GameMode >();

	// TEMP STUFF
	{
		GetGame()->GetInputManager()->SetLockCursor( true );
		GetGame()->GetInputManager()->SetShowCursor( false );

		//CreateObject< Car >();

		//CreateObject< Tiger >();

		// CreateObject< PacMan >();

		//CreateObject< FreeCamera >();

		//CreateObject< TestCameraPoint >();

		//TestCollisionShape::CreateTestObjects( *this );
	}
	// TEMP END

	// Do it after all objects are crated.
	// It checks collisions with all objects.
	m_navigationManager->StartUp( this );
}

void Level::ShutDown()
{
	assert( m_game );

	for( auto it = m_objects.begin(); it != m_objects.end(); ++it )
	{
		if( *it )
		{
			( *it )->Destroy();
			*it = nullptr;
		}
	}
	m_objects.clear();

	while( !m_registeredObjects.empty() )
	{
		m_registeredObjects.front()->Destroy();
	}

	// Game objects are destroyed with m_objects.
	m_gameMode = nullptr;
	m_terrain = nullptr;

	m_navigationManager->ShutDown();

	m_cameraManager->ShutDown();

	m_game = nullptr;
}

void Level::Update( float deltaTime )
{
	// Update objects.
	for( Object* object : m_objects )
	{
		if( object ) //< Object can be destroyed during update.
			object->Update( deltaTime );
	}

	// Make sure that new crated objects will be also updated.
	while( !m_registeredObjects.empty() )
	{
		auto firstIt = m_objects.insert( m_objects.end(), m_registeredObjects.begin(), m_registeredObjects.end() );
		m_registeredObjects.clear();

		// Update new added objects.
		for( auto it = firstIt; it != m_objects.end(); ++it )
		{
			if( *it ) //< Object can be destroyed during update.
				( *it )->Update( deltaTime );
		}
	}

	// DEBUG STUFF
	if( s_debugNavigationManager )
	{
		static int counter = 0;
		static D3DXVECTOR3 lastPlayerPosition( 0.0f, 0.0f, 0.0f );
		if( counter++ % 100 == 0 )
		{
			const Actor* player = GetGameMode()->GetPlayer();
			if( player )
			{
				D3DXVECTOR3 playerPosition = player->GetActorPosition();

				std::vector< D3DXVECTOR3 > path = m_navigationManager->FindPath( playerPosition, lastPlayerPosition );

				lastPlayerPosition = playerPosition;
			}
		}
	}
	// DEBUG END

	// Remove all nullptr.
	m_objects.erase( std::remove( m_objects.begin(), m_objects.end(), nullptr ), m_objects.end() );
}

void Level::Render( FrameRenderer& frame ) const
{
	m_cameraManager->Render();

	if( s_debugNavigationManager )
	{
		m_navigationManager->Render( frame );
	}

	for( Object* object : m_objects )
	{
		if( object )
			object->Render( frame );
	}
}

void Level::RegisterObject( Object* object )
{
	assert( object );
	assert( std::find( m_objects.begin(), m_objects.end(), object ) == m_objects.end() );
	assert( std::find( m_registeredObjects.begin(), m_registeredObjects.end(), object ) == m_registeredObjects.end() );

	m_registeredObjects.push_back( object );
}

void Level::UnregisterObject( Object* object )
{
	assert( object );
	assert( std::find( m_objects.begin(), m_objects.end(), object ) != m_objects.end() || std::find( m_registeredObjects.begin(), m_registeredObjects.end(), object ) != m_registeredObjects.end() );

	auto objIt = std::find( m_objects.begin(), m_objects.end(), object );
	if( objIt != m_objects.end() )
	{
		*objIt = nullptr;
	}
	else
	{
		m_registeredObjects.erase( std::find( m_registeredObjects.begin(), m_registeredObjects.end(), object ) );
	}
}

void Level::DestroyObject( Object* object )
{
	assert( object );
	object->Destroy();
}

Game* Level::GetGame() const
{
	assert( m_game );
	return m_game;
}

CameraManager* Level::GetCameraManager() const
{
	assert( m_cameraManager );
	return m_cameraManager.get();
}

Terrain* Level::GetTerrain() const
{
	assert( m_terrain );
	return m_terrain;
}

GameMode* Level::GetGameMode() const
{
	assert( m_gameMode );
	return m_gameMode;
}

std::vector< Object* > Level::GetAllObjects() const
{
	std::vector< Object* > allObjects;

	allObjects.reserve( m_objects.size() + m_registeredObjects.size() );

	allObjects.insert( allObjects.end(), m_objects.begin(), m_objects.end() );
	allObjects.insert( allObjects.end(), m_registeredObjects.begin(), m_registeredObjects.end() );

	return allObjects;
}

void Level::LoadTerrain()
{
	m_terrain = CreateObject< Terrain >( []( auto terrain ) {
		terrain->SetTerrainFile( "Content\\terrain.txt" );
	} );
}
