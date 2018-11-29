#include "FreeCamera.h"
#include "Actor.h"
#include "CameraManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"
#include "Projectile.h"

FreeCamera::FreeCamera()
	: m_yaw( 0.0f )
	, m_pitch( 30.0f )
	, m_position( 0.0f, 10.0f, 0.0f )
	, m_baseYawSpeed( 90.0f )
	, m_basePitchSpeed( 90.0f )
	, m_mouseYawSensitivity( 0.5f )
	, m_mousePitchSensitivity( 0.5f )
	, m_moveSpeed( 10.0f )
{
}

FreeCamera::~FreeCamera() = default;

void FreeCamera::OnUpdate( float deltaTime )
{
	UpdateCamera( deltaTime );

	Object::OnUpdate( deltaTime );
}

void FreeCamera::UpdateCamera( float deltaTime )
{
	Input input = GetInput( deltaTime );

	UpdateYaw( input.yaw );

	UpdatePitch( input.pitch );

	UpdateMovement( input.moveForward, input.moveRight, input.moveUp, deltaTime );

	if( input.fireProjectile )
	{
		FireProjectile();
	}

	SetCamera();
}

FreeCamera::Input FreeCamera::GetInput( float deltaTime ) const
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();

	Input input = {};


	input.yaw += inputMgr->GetMouseMoveX() * m_mouseYawSensitivity;

	if( inputMgr->IsKeyPressed( VK_LEFT ) )
	{
		input.yaw -= 1.0f * m_baseYawSpeed * deltaTime;
	}
	if( inputMgr->IsKeyPressed( VK_RIGHT ) )
	{
		input.yaw += 1.0f * m_baseYawSpeed * deltaTime;
	}


	input.pitch += inputMgr->GetMouseMoveY() * m_mousePitchSensitivity;

	if( inputMgr->IsKeyPressed( VK_UP ) )
	{
		input.pitch -= 1.0f * m_basePitchSpeed * deltaTime;
	}
	if( inputMgr->IsKeyPressed( VK_DOWN ) )
	{
		input.pitch += 1.0f * m_basePitchSpeed * deltaTime;
	}


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
		input.moveRight += 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'A' ) )
	{
		input.moveRight -= 1.0f;
	}


	if( inputMgr->IsKeyPressed( 'E' ) )
	{
		input.moveUp += 1.0f;
	}
	if( inputMgr->IsKeyPressed( 'Q' ) )
	{
		input.moveUp -= 1.0f;
	}


	if( inputMgr->IsKeyJustPressed( MK_LBUTTON ) )
	{
		input.fireProjectile = true;
	}


	return input;
}

void FreeCamera::UpdateYaw( float yawInput )
{
	m_yaw = fmodf( m_yaw + yawInput, 360.0f );
}

void FreeCamera::UpdatePitch( float pitchInput )
{
	m_pitch = Math::Clamp( m_pitch + pitchInput, -89.0f, +89.0f );
}

void FreeCamera::UpdateMovement( float moveForwardInput, float moveRightInput, float moveUpInput, float deltaTime )
{
	D3DXVECTOR3 forward = GetCameraDirection();
	D3DXVECTOR3 right = Math::GetPerpendicularVector2d( forward );
	D3DXVec3Normalize( &right, &right );
	const D3DXVECTOR3& up = Math::s_upVector3;

	m_position += forward * moveForwardInput * deltaTime * m_moveSpeed;
	m_position += right * moveRightInput * deltaTime * m_moveSpeed;
	m_position += up * moveUpInput * deltaTime * m_moveSpeed;
}

void FreeCamera::FireProjectile()
{
	GetLevel()->CreateObject< Projectile >( [ this ]( auto projectile )
	{
		projectile->SetRadius( 0.5f );
		projectile->SetGravityFactor( 1.0f );
		projectile->SetLifeTime( 5.0f );
		projectile->SetActorPosition( m_position );
		projectile->SetVelocity( GetCameraDirection() * 25.0f );
	} );
}

void FreeCamera::SetCamera()
{
	D3DXVECTOR3 direction = GetCameraDirection();

	CameraManager* cameraManger = GetLevel()->GetCameraManager();
	cameraManger->SetPositionDirection( m_position, direction );
}

D3DXVECTOR3 FreeCamera::GetCameraDirection() const
{
	D3DXMATRIX matCamera;
	D3DXMatrixRotationYawPitchRoll( &matCamera, Math::Deg2Rad( m_yaw ), Math::Deg2Rad( m_pitch ), 0.0f );
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal( &direction, &Math::s_forwardVector3, &matCamera );
	return direction;
}
