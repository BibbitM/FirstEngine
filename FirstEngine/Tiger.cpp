#include "Tiger.h"
#include "GenericCamera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

Tiger::Tiger()
	: m_camera( nullptr )
	, m_moveAcceleration( 10.0f )
	, m_moveDeceleration( 1000000.0f )
	, m_moveSpeedMax( 10.0f )
	, m_moveAirControl( 0.1f )
	, m_jumpSpeed( 5.0f )
	, m_jumpGravity( 10.0f )
	, m_rotationChangeSpeed( Math::Deg2Rad( 180.0f ) )
	, m_rotationAirControl( 0.5f )
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

	m_camera = GetLevel()->CreateObject< GenericCamera >( [this]( auto camera ) {
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

	UpdateMovement( input.moveForward, input.moveRight, input.jump, deltaTime );
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


	if( inputMgr->IsKeyJustPressed( VK_SPACE ) )
	{
		input.jump = true;
	}


	return input;
}

void Tiger::UpdateMovement( float moveForwardInput, float moveRightInput, bool jumpInput, float deltaTime )
{
	D3DXVECTOR3 velocity = GetVelocity();
	// Extract y (up/down) velocity to be calculated separately.
	float velocityY = velocity.y;
	velocity.y = 0.0f;


	// Calculate 2D velocity (x, z).

	const float controlFactor = GetTouchesGround() ? 1.0f : m_moveAirControl;

	if( moveForwardInput == 0.0f && moveRightInput == 0.0f )
	{
		float speed = D3DXVec3Length( &velocity );
		speed = Math::InterpolateTo( speed, 0.0f, deltaTime, m_moveDeceleration * controlFactor );
		if( speed <= 0.0f )
		{
			velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
		else
		{
			D3DXVec3Normalize( &velocity, &velocity );
			velocity *= speed;
		}
	}
	else
	{
		D3DXVECTOR3 velocityChangeInput( 0.0f, 0.0f, 0.0f );
		velocityChangeInput += GetActorWorldForwardVector() * moveForwardInput;
		velocityChangeInput += GetActorWorldRightVector() * moveRightInput;

		float speedInput = D3DXVec3Length( &velocityChangeInput );

		D3DXVec3Normalize( &velocityChangeInput, &velocityChangeInput );
		D3DXVECTOR3 velocityPendicularInput = Math::GetPerpendicularVector2d( velocityChangeInput );

		float forwardSpeed = D3DXVec3Dot( &velocity, &velocityChangeInput );
		float sideSpeed = D3DXVec3Dot( &velocity, &velocityPendicularInput );

		forwardSpeed += speedInput * m_moveAcceleration * controlFactor * deltaTime;
		sideSpeed = Math::InterpolateTo( sideSpeed, 0.0f, deltaTime, m_moveDeceleration );

		velocity = velocityChangeInput * forwardSpeed + velocityPendicularInput * sideSpeed;
	

		if( m_moveSpeedMax > 0.0f && D3DXVec3Length( &velocity ) > m_moveSpeedMax )
		{
			D3DXVec3Normalize( &velocity, &velocity );
			velocity *= m_moveSpeedMax;
		}
	}


	// Calculate up/down velocity (y).

	if( GetTouchesGround() )
	{
		velocityY = 0.0f;
	}

	if( GetTouchesGround() && jumpInput )
	{
		velocityY += m_jumpSpeed;
	}
	else
	{
		velocityY -= m_jumpGravity * deltaTime;
	}


	velocity.y = velocityY;
	SetVelocity( velocity );
}

void Tiger::UpdateRotation( float rotationInput, float deltaTime )
{
	const float controlFactor = GetTouchesGround() ? 1.0f : m_rotationAirControl;

	D3DXVECTOR3 rotation = GetActorRotation();
	rotation.y += rotationInput * m_rotationChangeSpeed * controlFactor * deltaTime;
	SetActorRotation( rotation );
}
