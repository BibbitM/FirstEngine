#include "Camera.h"
#include "Actor.h"
#include "CameraManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"

Camera::Camera()
	: m_target( nullptr )
	, m_yaw( 0.0f )
	, m_pitch( 0.0f )
	, m_currentDistance( 5.0f )
	, m_wantedDistance( 5.0f )
	, m_baseYawSpeed( 90.0f )
	, m_basePitchSpeed( 45.0f )
	, m_distanceChangeStep( 1.0f )
	, m_distanceMin( 1.0f )
	, m_distanceMax( 10.0f )
	, m_distanceChangeSpeed( 5.0f )
{
}

Camera::~Camera() = default;

D3DXVECTOR3 Camera::GetTargetPosition() const
{
	if (m_target)
	{
		return m_target->GetActorPosition();
	}
	else
	{
		return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
}

void Camera::OnUpdate( float deltaTime )
{
	UpdateCamera( deltaTime );

	Object::OnUpdate( deltaTime );
}

void Camera::UpdateCamera( float deltaTime )
{
	Input input = GetCameraInput( deltaTime );

	UpdateYaw( input.yaw );

	UpdatePitch( input.pitch );

	UpdateDistance( deltaTime, input.distance );


	CameraManager* cameraManger = GetLevel()->GetCameraManager();

	D3DXVECTOR3 direction = cameraManger->GetDirection();
	D3DXVECTOR3 position = GetTargetPosition() - direction * m_currentDistance;
	position.y += 0.75f;
	cameraManger->SetPositionDirection( position, direction );
}

Camera::Input Camera::GetCameraInput( float deltaTime ) const
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();

	Input input = {};


	input.pitch += inputMgr->GetMouseMoveX();

	if( inputMgr->IsKeyPressed( VK_UP ) )
	{
		input.pitch -= 1.0f * m_basePitchSpeed * deltaTime;
	}
	if( inputMgr->IsKeyPressed( VK_DOWN ) )
	{
		input.pitch += 1.0f * m_basePitchSpeed * deltaTime;
	}


	input.yaw += inputMgr->GetMouseMoveY();

	if( inputMgr->IsKeyPressed( VK_LEFT ) )
	{
		input.yaw += 1.0f * m_baseYawSpeed * deltaTime;
	}
	if( inputMgr->IsKeyPressed( VK_RIGHT ) )
	{
		input.yaw -= 1.0f * m_baseYawSpeed * deltaTime;
	}


	input.distance -= inputMgr->GetMouseWheel() * m_distanceChangeStep;


	return input;
}

void Camera::UpdateYaw( float yawInput )
{
}

void Camera::UpdatePitch( float pitchInput )
{
}

void Camera::UpdateDistance( float deltaTime, float distanceInput )
{
	m_wantedDistance = Math::Clamp( m_wantedDistance + distanceInput, m_distanceMin, m_distanceMax );

	m_currentDistance = Math::InterpolateTo( m_currentDistance, m_wantedDistance, deltaTime, m_distanceChangeSpeed );
}

void Camera::SetCamera()
{
}

D3DXVECTOR3 Camera::GetCameraDir() const
{
	return D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
}
