#include "Level.h"
#include "CameraManager.h"
#include "Object.h"
#include <algorithm>
#include <cassert>

// TEMP STUFF
#include "Game.h"
#include "InputManager.h"
#include "Pawn.h"
#include "TestCameraPoint.h"
#include "Tiger.h"
// TEMP END


Level::Level()
	: m_game( nullptr )
	, m_cameraManager( std::make_unique< CameraManager >() )
{
}

Level::~Level()
{
	assert( !m_game );
	assert( m_objects.empty() );
	assert( m_registeredObjects.empty() );
}

void Level::StartUp( Game* game )
{
	assert( game );
	assert( !m_game );
	m_game = game;

	m_cameraManager->StartUp( this );

	// TEMP STUFF
	{
		GetGame()->GetInputManager()->SetLockCursor( false );
		GetGame()->GetInputManager()->SetShowCursor( true );

		CreateObject< Pawn >( []( auto floor ) {
			floor->SetMesh( "Content\\plane.x" );
			floor->SetTexture( "Content\\lava.jpg" );
			floor->SetActorScale( D3DXVECTOR3( 10.0f, 1.0f, 10.0f ) );
		} );

		CreateObject< Tiger >();

		CreateObject< TestCameraPoint >();
	}
	// TEMP END
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

	// Remove all nullptr.
	m_objects.erase( std::remove( m_objects.begin(), m_objects.end(), nullptr ), m_objects.end() );
}

void Level::Render( FrameRenderer& frame ) const
{
	m_cameraManager->Render();

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
