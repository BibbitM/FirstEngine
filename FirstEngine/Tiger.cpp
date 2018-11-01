#include "Tiger.h"
#include "Camera.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include <cassert>

Tiger::Tiger()
	: m_camera( nullptr )
{
	// Setup mesh.
	SetMesh( "Content\\tiger.x" );
	SetTexture( "Content\\tiger.bmp" );
	SetMeshPosition( D3DXVECTOR3( 0.0f, 0.75f, 0.0f ) );
}

Tiger::~Tiger() = default;

void Tiger::OnStartUp()
{
	Pawn::OnStartUp();

	m_camera = GetLevel()->CreateObject< Camera >( [this]( auto camera )
	{
		camera->SetTarget( this );
	} );
}

void Tiger::OnShutDown()
{
	assert( m_camera );
	m_camera->Destroy();
	m_camera = nullptr;

	Pawn::OnShutDown();
}

void Tiger::OnUpdate( float deltaTime )
{
	UpdateRotation( deltaTime );

	Pawn::OnUpdate( deltaTime );
}

void Tiger::UpdateRotation( float deltaTime )
{
	float rotationInput = GetRotationInput();

	static const float rotationChangeSpeed = Math::Deg2Rad( 90.0f );

	D3DXVECTOR3 rotation = GetActorRotation();
	rotation.y += rotationInput * rotationChangeSpeed * deltaTime;
	SetActorRotation( rotation );
}

float Tiger::GetRotationInput() const
{
	const InputManager* inputs = GetLevel()->GetGame()->GetInputManager();

	float rotationInput = 0.0f;
	if( inputs->IsKeyPressed( 'A' ) )
	{
		rotationInput -= 1.0f;
	}
	if( inputs->IsKeyPressed( 'D' ) )
	{
		rotationInput += 1.0f;
	}

	return rotationInput;
}
