#include "Tiger.h"
#include "Camera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

Tiger::Tiger()
	: m_camera( nullptr )
	, m_moveAcceleration( 10.0f )
	, m_moveDeceleration( 100.0f )
	, m_moveSpeedMax( 10.0f )
	, m_rotationChangeSpeed( Math::Deg2Rad( 180.0f ) )
{
	// Setup mesh.
	SetMesh( "Content\\tiger.x" );
	SetTexture( "Content\\tiger.bmp" );
	SetMeshPosition( D3DXVECTOR3( 0.0f, 0.75f, 0.0f ) );
	SetMeshRotation( D3DXVECTOR3( 0.0f, Math::Deg2Rad( 180.0f ), 0.0f ) );
}

Tiger::~Tiger() = default;

void Tiger::OnStartUp()
{
	Character::OnStartUp();

	m_camera = GetLevel()->CreateObject< Camera >( [this]( auto camera ) {
		camera->SetTarget( this );
	} );
}

void Tiger::OnShutDown()
{
	assert( m_camera );
	m_camera->Destroy();
	m_camera = nullptr;

	Character::OnShutDown();
}

void Tiger::OnUpdate( float deltaTime )
{
	UpdateTiger( deltaTime );

	Character::OnUpdate( deltaTime );
}

void Tiger::UpdateTiger( float deltaTime )
{
	Input input = GetInput();

	UpdateRotation( input.rotation, deltaTime );

	UpdateMovement( input.moveForward, input.moveRight, deltaTime );
}

Tiger::Input Tiger::GetInput() const
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();

	Input input = {};


	if( inputMgr->IsKeyPressed( 'W' ) )
	{
		input.moveForward += 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'S' ) )
	{
		input.moveForward -= 1.0f;
	}


	if( inputMgr->IsKeyPressed( 'E' ) )
	{
		input.moveRight += 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'Q' ) )
	{
		input.moveRight -= 1.0f;
	}


	if( inputMgr->IsKeyPressed( 'A' ) )
	{
		input.rotation -= 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'D' ) )
	{
		input.rotation += 1.0f;
	}


	return input;
}

void Tiger::UpdateMovement( float moveForwardInput, float moveRightInput, float deltaTime )
{
	D3DXVECTOR3 velocity = GetVelocity();

	if( moveForwardInput == 0.0f && moveRightInput == 0.0f )
	{
		velocity = Math::InterpolateTo( velocity, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), deltaTime, m_moveDeceleration );
	}
	else
	{
		D3DXVECTOR3 velocityChange( 0.0f, 0.0f, 0.0f );
		velocityChange += GetActorForwardVector() * moveForwardInput;
		velocityChange += GetActorRightVector() * moveRightInput;

		velocity = Math::InterpolateTo( velocity, velocity + velocityChange, deltaTime, m_moveAcceleration );

		if( m_moveSpeedMax > 0.0f && D3DXVec3Length( &velocity ) > m_moveSpeedMax )
		{
			D3DXVec3Normalize( &velocity, &velocity );
			velocity *= m_moveSpeedMax;
		}
	}

	SetVelocity( velocity );
}

void Tiger::UpdateRotation( float rotationInput, float deltaTime )
{
	D3DXVECTOR3 rotation = GetActorRotation();
	rotation.y += rotationInput * m_rotationChangeSpeed * deltaTime;
	SetActorRotation( rotation );
}
