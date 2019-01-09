#include "PacMan.h"
#include "GenericCamera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

PacMan::PacMan()
	: m_camera( nullptr )
{
	// Setup mesh.
	SetMesh( "Content\\unitsphere.x" );
	SetColor( 0x00FFFF00 );
	SetRadius( 0.5f );
	SetBounceFactor( 0.2f );
}

PacMan::~PacMan()
{
	assert( !m_camera );
}

void PacMan::OnStartUp()
{
	PhysicsCharacter::OnStartUp();

	m_camera = GetLevel()->CreateObject< GenericCamera >( [this]( auto camera ) {
		camera->SetTarget( this );
	} );
}

void PacMan::OnShutDown()
{
	assert( m_camera );
	m_camera->Destroy();
	m_camera = nullptr;

	PhysicsCharacter::OnShutDown();
}

void PacMan::OnUpdate( float deltaTime )
{
	UpdateInput();

	PhysicsCharacter::OnUpdate( deltaTime );
}

void PacMan::UpdateInput()
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();


	D3DXVECTOR3 forward = m_camera->GetCameraDirection();
	forward.y = 0.0f;
	D3DXVec3Normalize( &forward, &forward );

	D3DXVECTOR3 right = Math::GetPerpendicularVector2d( forward );


	if( inputMgr->IsKeyPressed( 'W' ) )
	{
		AddMovementInput( forward );
	}
	if( inputMgr->IsKeyPressed( 'S' ) )
	{
		AddMovementInput( -forward );
	}


	if( inputMgr->IsKeyPressed( 'A' ) )
	{
		AddMovementInput( -right );
	}
	if( inputMgr->IsKeyPressed( 'D' ) )
	{
		AddMovementInput( right );
	}


	if( inputMgr->IsKeyJustPressed( VK_SPACE ) )
	{
		AddJumpInput();
	}
}
