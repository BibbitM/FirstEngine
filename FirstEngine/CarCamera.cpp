#include "CarCamera.h"
#include "Actor.h"
#include "CameraManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "Math.h"

CarCamera::CarCamera()
	: m_target( nullptr )
	, m_previousTargetPosition( 0.0f, 0.0f, 0.0f )
	, m_previousTargetPositionSet( false )
	, m_yaw( 0.0f )
	, m_pitch( 30.0f )
	, m_currentDistance( 5.0f )
	, m_wantedDistance( 5.0f )
	, m_baseYawSpeed( 90.0f )
	, m_basePitchSpeed( 90.0f )
	, m_mouseYawSensitivity( 0.5f )
	, m_mousePitchSensitivity( 0.5f )
	, m_distanceChangeStep( 1.0f )
	, m_distanceMin( 1.0f )
	, m_distanceMax( 10.0f )
	, m_distanceChangeSpeed( 5.0f )
{
}

CarCamera::~CarCamera() = default;

void CarCamera::SetTarget( const Actor* target )
{
	m_target = target;
	m_previousTargetPositionSet = false;
}

D3DXVECTOR3 CarCamera::GetTargetPosition() const
{
	if (m_target)
	{
		D3DXVECTOR3 targetPosition = m_target->GetActorPosition();
		targetPosition.y += 1.0f; //< HACK: We want look at "head" position, not "legs".
		return targetPosition;
	}
	else
	{
		return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
}

void CarCamera::OnUpdate( float deltaTime )
{
	UpdateCamera( deltaTime );

	Object::OnUpdate( deltaTime );
}

void CarCamera::UpdateCamera( float deltaTime )
{
	Input input = GetInput( deltaTime );

	UpdateYawFromTargetMovement();

	UpdateYaw( input.yaw );

	UpdatePitch( input.pitch );

	UpdateDistance( deltaTime, input.distance );

	SetCamera();

	StoreTargetPosition();
}

CarCamera::Input CarCamera::GetInput( float deltaTime ) const
{
	const InputManager* inputMgr = GetLevel()->GetGame()->GetInputManager();

	Input input = {};


	input.yaw += inputMgr->GetMouseMoveX() * m_mouseYawSensitivity;

	if( inputMgr->IsKeyPressed( VK_LEFT ) )
	{
		input.yaw += 1.0f * m_baseYawSpeed * deltaTime;
	}
	if( inputMgr->IsKeyPressed( VK_RIGHT ) )
	{
		input.yaw -= 1.0f * m_baseYawSpeed * deltaTime;
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


	input.distance -= inputMgr->GetMouseWheel() * m_distanceChangeStep;


	return input;
}

void CarCamera::UpdateYawFromTargetMovement()
{
	if( !m_previousTargetPositionSet )
	{
		return;
	}

	D3DXVECTOR3 prevDirection = GetCameraDirection();
	prevDirection.y = 0.0f;
	D3DXVec3Normalize( &prevDirection, &prevDirection );

	D3DXVECTOR3 eyePosition = m_previousTargetPosition - prevDirection * m_currentDistance;

	D3DXVECTOR3 currDirection = GetTargetPosition() - eyePosition;
	currDirection.y = 0.0f;
	D3DXVec3Normalize( &currDirection, &currDirection );

	D3DXVECTOR3 directionCross;
	D3DXVec3Cross( &directionCross, &prevDirection, &currDirection );

	float yawChangeSin = directionCross.y;
	// Add clamp. If value is slightly greater than 1 we will get NAN in next line.
	yawChangeSin = Math::Clamp( yawChangeSin, -1.0f, +1.0f );
	float yawChange = Math::Rad2Deg( asinf( yawChangeSin ) );

	UpdateYaw( yawChange );
}

void CarCamera::UpdateYaw( float yawInput )
{
	m_yaw = fmodf( m_yaw + yawInput, 360.0f );
}

void CarCamera::UpdatePitch( float pitchInput )
{
	m_pitch = Math::Clamp( m_pitch + pitchInput, -89.0f, +89.0f );
}

void CarCamera::UpdateDistance( float deltaTime, float distanceInput )
{
	m_wantedDistance = Math::Clamp( m_wantedDistance + distanceInput, m_distanceMin, m_distanceMax );

	m_currentDistance = Math::InterpolateTo( m_currentDistance, m_wantedDistance, deltaTime, m_distanceChangeSpeed );
}

void CarCamera::StoreTargetPosition()
{
	m_previousTargetPosition = GetTargetPosition();
	m_previousTargetPositionSet = true;
}

void CarCamera::SetCamera()
{
	D3DXVECTOR3 direction = GetCameraDirection();
	D3DXVECTOR3 position = GetTargetPosition() - direction * m_currentDistance;

	CameraManager* cameraManger = GetLevel()->GetCameraManager();
	cameraManger->SetPositionDirection( position, direction );
}

D3DXVECTOR3 CarCamera::GetCameraDirection() const
{
	D3DXMATRIX matCamera;
	D3DXMatrixRotationYawPitchRoll( &matCamera, Math::Deg2Rad( m_yaw ), Math::Deg2Rad( m_pitch ), 0.0f );
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal( &direction, &Math::s_forwardVector3, &matCamera );
	return direction;
}
