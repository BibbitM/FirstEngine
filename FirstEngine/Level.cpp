#include "Level.h"
#include "CameraManager.h"
#include "Object.h"
#include <algorithm>
#include <cassert>

// TEMP STUFF
#include "FrameRenderer.h"
#include "Game.h"
#include "InputManager.h"
#include "Math.h"
#include "Pawn.h"
// TEMP END


Level::Level()
	: m_game( nullptr )
	, m_cameraManager( std::make_unique< CameraManager >() )
	// TEMP STUFF
	, m_tiger( nullptr )
	, m_cameraDistance( 5.0f )
// TEMP END
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
		CreateObject< Pawn >( []( auto floor )
		{
			floor->SetMesh( "Content\\plane.x" );
			floor->SetTexture( "Content\\lava.jpg" );
			floor->SetActorScale( D3DXVECTOR3( 10.0f, 1.0f, 10.0f ) );
		} );

		m_tiger = CreateObject< Pawn >( []( auto tiger )
		{
			tiger->SetMesh( "Content\\tiger.x" );
			tiger->SetTexture( "Content\\tiger.bmp" );
			tiger->SetMeshPosition( D3DXVECTOR3( 0.0f, 0.75f, 0.0f ) );
		} );
	}
	// TEMP END
}

void Level::ShutDown()
{
	assert( m_game );

	// TEMP STUFF
	{
		m_tiger->Destroy();
		m_tiger = nullptr;
	}
	// TEMP END

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
	// TEMP STUFF
	{
		const InputManager* inputs = GetGame()->GetInputManager();

		{
			float tigerRotChange = 0;
			if( inputs->IsKeyPressed( 'A' ) )
			{
				tigerRotChange -= 1.0f;
			}
			if( inputs->IsKeyPressed( 'D' ) )
			{
				tigerRotChange += 1.0f;
			}

			static const float tigerRotChangeSpeed = Math::Deg2Rad( 90.0f );

			D3DXVECTOR3 tigerRotation = m_tiger->GetActorRotation();
			tigerRotation.y += tigerRotChange * tigerRotChangeSpeed * deltaTime;
			m_tiger->SetActorRotation( tigerRotation );
		}

		{
			float camDistChange = 0.0f;
			if( inputs->IsKeyPressed( VK_UP ) )
			{
				camDistChange -= 1.0f;
			}
			if( inputs->IsKeyPressed( VK_DOWN ) )
			{
				camDistChange += 1.0f;
			}

			static const float camDistChangeSpeed = 5.0f;
			static const float camDistMin = 1.0f;
			static const float camDistMax = 10.0f;

			m_cameraDistance += camDistChange * camDistChangeSpeed * deltaTime;
			m_cameraDistance = Math::Clamp( m_cameraDistance, camDistMin, camDistMax );

			D3DXVECTOR3 camDir = m_cameraManager->GetDirection();
			D3DXVECTOR3 camPos = m_tiger->GetActorPosition() - camDir * m_cameraDistance;
			camPos.y += 0.75f;
			m_cameraManager->SetPositionDirection( camPos, camDir );
		}
	}
	// TEMP END

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
