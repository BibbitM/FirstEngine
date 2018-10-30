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
#include "MeshManager.h"
#include "TextureManager.h"
// TEMP END


Level::Level()
	: m_game( nullptr )
	, m_cameraManager( std::make_unique< CameraManager >() )
	// TEMP STUFF
	, m_tigerPosition( 0.0f, 0.75f, 0.0f )
	, m_tigerAngle( 0.0f )
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
		m_planeMesh = GetGame()->GetMeshManager()->LoadMesh( "Content\\plane.x" );
		m_tigerMesh = GetGame()->GetMeshManager()->LoadMesh( "Content\\tiger.x" );

		m_lavaTexture = GetGame()->GetTextureManager()->LoadTexture( "Content\\lava.jpg" );
		m_tigerTexture = GetGame()->GetTextureManager()->LoadTexture( "Content\\tiger.bmp" );
	}
	// TEMP END
}

void Level::ShutDown()
{
	assert( m_game );

	// TEMP STUFF
	{
		GetGame()->GetMeshManager()->UnloadMesh( m_planeMesh );
		m_planeMesh.clear();
		GetGame()->GetMeshManager()->UnloadMesh( m_tigerMesh );
		m_tigerMesh.clear();

		GetGame()->GetTextureManager()->UnloadTexture( m_lavaTexture );
		m_lavaTexture.clear();
		GetGame()->GetTextureManager()->UnloadTexture( m_tigerTexture );
		m_tigerTexture.clear();
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

			static const float tigerRotChangeSpeed = 90.0f;

			m_tigerAngle += tigerRotChange * tigerRotChangeSpeed * deltaTime;
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
			D3DXVECTOR3 camPos = m_tigerPosition - camDir * m_cameraDistance;
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

	// TEMP STUFF
	{
		/*
		D3DXMATRIX matRotX, matRotY, matRotZ, matTrans;

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, rot.x ); // Pitch
		D3DXMatrixRotationY( &matRotY, rot.y ); // Yaw
		D3DXMatrixRotationZ( &matRotZ, rot.z ); // Roll

		// Calculate a translation matrix
		D3DXMatrixTranslation( &matTrans, m_position.x, m_position.y, m_position.z );

		// Calculate our world matrix by multiplying the above (in the correct order)
		D3DXMATRIX matWorld = ( matRotX * matRotY * matRotZ ) * matTrans;
		*/

		static const D3DCOLOR whiteColor = 0xFFFFFFFF;

		D3DXVECTOR3 floorScale( 10.0f, 1.0f, 10.0f );
		D3DXMATRIX floorMat;
		D3DXMatrixScaling( &floorMat, floorScale.x, floorScale.y, floorScale.z );

		frame.AddMesh( floorMat, m_planeMesh, m_lavaTexture, whiteColor );


		D3DXMATRIX tigerRotMat, tigerTransMat;
		D3DXMatrixRotationY( &tigerRotMat, Math::Deg2Rad( m_tigerAngle ) );
		D3DXMatrixTranslation( &tigerTransMat, m_tigerPosition.x, m_tigerPosition.y, m_tigerPosition.z );
		D3DXMATRIX tigerMat = tigerRotMat * tigerTransMat;

		frame.AddMesh( tigerMat, m_tigerMesh, m_tigerTexture, whiteColor );
	}
	// TEMP END

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
