#include "Tiger.h"
#include "GenericCamera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

Tiger::Tiger()
	: m_camera( nullptr )
	, m_rotationChangeSpeed( Math::Deg2Rad( 180.0f ) )
	, m_rotationAirControl( 0.5f )
{
	// Setup mesh.
	SetMesh( "Content\\tiger.x" );
	SetTexture( "Content\\tiger.bmp" );
	SetRadius( 1.0f );
	SetBounceFactor( 0.0f );
	SetMeshPosition( D3DXVECTOR3( 0.0f, 0.75f - 1.0f, 0.0f ) );
	SetMeshRotation( D3DXVECTOR3( 0.0f, Math::Deg2Rad( 180.0f ), 0.0f ) );
}

Tiger::~Tiger()
{
	assert( !m_camera );
}

void Tiger::OnStartUp()
{
	PhysicsCharacter::OnStartUp();

	m_camera = GetLevel()->CreateObject< GenericCamera >( [this]( auto camera ) {
		camera->SetTarget( this );
	} );
}

void Tiger::OnShutDown()
{
	assert( m_camera );
	m_camera->Destroy();
	m_camera = nullptr;

	PhysicsCharacter::OnShutDown();
}

void Tiger::OnUpdate( float deltaTime )
{
	UpdateTiger( deltaTime );

	PhysicsCharacter::OnUpdate( deltaTime );
}

void Tiger::UpdateTiger( float deltaTime )
{
	float rotationInput = UpdateInput();

	UpdateRotation( rotationInput, deltaTime );
}

float Tiger::UpdateInput()
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();

	float rotationInput = 0.0f;


	if( inputMgr->IsKeyPressed( 'W' ) )
	{
		AddMovementInput( GetActorWorldForwardVector() );
	}
	if( inputMgr->IsKeyPressed( 'S' ) )
	{
		AddMovementInput( -GetActorWorldForwardVector() );
	}


	if( inputMgr->IsKeyPressed( 'E' ) )
	{
		AddMovementInput( GetActorWorldRightVector() );
	}
	if( inputMgr->IsKeyPressed( 'Q' ) )
	{
		AddMovementInput( -GetActorWorldRightVector() );
	}


	if( inputMgr->IsKeyJustPressed( VK_SPACE ) )
	{
		AddJumpInput();
	}


	if( inputMgr->IsKeyPressed( 'A' ) )
	{
		rotationInput -= 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'D' ) )
	{
		rotationInput += 1.0f;
	}


	return rotationInput;
}

void Tiger::UpdateRotation( float rotationInput, float deltaTime )
{
	const float controlFactor = GetTouchesGround() ? 1.0f : m_rotationAirControl;

	D3DXVECTOR3 rotation = GetActorRotation();
	rotation.y += rotationInput * m_rotationChangeSpeed * controlFactor * deltaTime;
	SetActorRotation( rotation );
}
