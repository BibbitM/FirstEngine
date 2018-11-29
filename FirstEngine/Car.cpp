#include "Car.h"
#include "GenericCamera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

Car::Car()
	: m_camera( nullptr )
	, m_moveAcceleration( 25.0f )
	, m_moveDeceleration( 5.0f )
	, m_moveSideDeceleration( 100.0f )
	, m_moveInAirDeceleration( 1.0f )
	, m_moveSpeedMax( 25.0f )
	, m_gravity( 10.0f )
	, m_rotationChangeSpeed( Math::Deg2Rad( 180.0f ) )
{
	SetMesh( "Content\\unitbox.x" );
	SetColor( 0xFFCC0505 );
	SetMeshScale( D3DXVECTOR3( 1.0f, 0.5f, 2.5f ) );
	SetMeshPosition( D3DXVECTOR3( 0.0f, 0.3125f, 0.0f ) );
}

Car::~Car() = default;

void Car::OnStartUp()
{
	Character::OnStartUp();

	// Create camera.
	m_camera = GetLevel()->CreateObject< GenericCamera >( [ this ]( auto camera ) {
		camera->SetTarget( this );
	} );

	// Create car parts.

	// Wheels.
	for( float x = -1.0f; x <= 1.0f; x += 2.0f )
	{
		for( float z = -1.0f; z <= 1.0f; z += 2.0f )
		{
			GetLevel()->CreateObject< Pawn >( [ this, x, z ]( auto pawn ) {
				pawn->SetMesh( "Content\\unitsphere.x" );
				pawn->SetColor( 0xFF101010 );
				pawn->SetActorPosition( D3DXVECTOR3( x * 0.525f, 0.2f, z * 0.9f ) );
				pawn->SetActorScale( D3DXVECTOR3( 0.1f, 0.4f, 0.4f ) );
				pawn->AttachTo( this );
			} );
		}
	}

	// Cabin.
	GetLevel()->CreateObject< Pawn >( [ this ]( auto pawn ) {
		pawn->SetMesh( "Content\\unitbox.x" );
		pawn->SetColor( 0xFF2222DD );
		pawn->SetMeshScale( D3DXVECTOR3( 0.75f, 0.375f, 1.0f ) );
		pawn->SetMeshPosition( D3DXVECTOR3( 0.0f, 0.3125f * 2, -0.25f ) );
		pawn->AttachTo( this );
	} );
}

void Car::OnShutDown()
{
	assert( m_camera );
	m_camera->Destroy();
	m_camera = nullptr;

	Character::OnShutDown();
}

void Car::OnUpdate( float deltaTime )
{
	UpdateCar( deltaTime );

	Character::OnUpdate( deltaTime );
}

void Car::UpdateCar( float deltaTime )
{
	Input input = GetInput();

	UpdateRotation( input.turnRight, deltaTime );

	UpdateMovement( input.moveForward, deltaTime );
}

Car::Input Car::GetInput() const
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


	if( inputMgr->IsKeyPressed( 'D' ) )
	{
		input.turnRight += 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'A' ) )
	{
		input.turnRight -= 1.0f;
	}


	return input;
}

void Car::UpdateMovement( float moveForwardInput, float deltaTime )
{
	D3DXVECTOR3 velocity = GetVelocity();
	// Extract y (up/down) velocity to be calculated separately.
	float velocityY = velocity.y;
	velocity.y = 0.0f;


	// Calculate 2D velocity (x, z).

	if( GetTouchesGround() )
	{
		const D3DXVECTOR3 actorForward = GetActorWorldForwardVector();
		const D3DXVECTOR3 actorRight = GetActorWorldRightVector();

		float forwardSpeed = D3DXVec3Dot( &actorForward, &GetVelocity() );	
		float rightSpeed = D3DXVec3Dot( &actorRight, &GetVelocity() );

		if( moveForwardInput != 0.0f )
		{
			forwardSpeed += moveForwardInput * m_moveAcceleration * deltaTime;
		}
		else
		{
			forwardSpeed = Math::InterpolateTo( forwardSpeed, 0.0f, deltaTime, m_moveDeceleration );
		}

		rightSpeed = Math::InterpolateTo( rightSpeed, 0.0f, deltaTime, m_moveSideDeceleration );


		velocity = forwardSpeed * actorForward + rightSpeed * actorRight;

		if( m_moveSpeedMax > 0.0f && D3DXVec3Length( &velocity ) > m_moveSpeedMax )
		{
			D3DXVec3Normalize( &velocity, &velocity );
			velocity *= m_moveSpeedMax;
		}
	}
	else
	{
		float speed = D3DXVec3Length( &velocity );
		speed = Math::InterpolateTo( speed, 0.0f, deltaTime, m_moveInAirDeceleration );
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

	// Calculate up/down velocity (y).

	if( GetTouchesGround() )
	{
		velocityY = 0.0f;
	}

	velocityY -= m_gravity * deltaTime;


	velocity.y = velocityY;
	SetVelocity( velocity );
}

void Car::UpdateRotation( float rotationInput, float deltaTime )
{
	if( !GetTouchesGround() )
	{
		return;
	}

	float sign = 1.0f;

	const D3DXVECTOR3 actorForward = GetActorWorldForwardVector();
	if( D3DXVec3Dot( &actorForward, &GetVelocity() ) < 0 )
	{
		// Moving backwards.
		sign = -1.0f;
	}

	D3DXVECTOR3 rotation = GetActorRotation();
	rotation.y += rotationInput * m_rotationChangeSpeed * deltaTime * D3DXVec3Length( &GetVelocity() ) / m_moveSpeedMax * sign;
	SetActorRotation( rotation );
}
