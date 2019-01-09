#include "PhysicsCharacter.h"
#include "CollisionResult.h"
#include "LevelCollisions.h"
#include "Math.h"
#include "ShapeSphere.h"

PhysicsCharacter::PhysicsCharacter()
	: m_radius( 0.05f )
	, m_bounceFactor( 0.0f )
	, m_moveAcceleration( 100.0f )
	, m_moveDeceleration( 100.0f )
	, m_moveSpeedMax( 10.0f )
	, m_moveAirControl( 0.1f )
	, m_jumpSpeed( 5.0f )
	, m_jumpGravity( 10.0f )
	, m_movementInput( 0.0f, 0.0f, 0.f )
	, m_jumpInput( false )
{
}

PhysicsCharacter::~PhysicsCharacter() = default;


void PhysicsCharacter::OnUpdate( float deltaTime )
{
	// Update inputs before super implementation.
	UpdateMovement( deltaTime );

	Character::OnUpdate( deltaTime );
}

void PhysicsCharacter::UpdateActorPosition( float deltaTime )
{
	// Do not call super implementation.

	static const float minimalMoveLen = 0.00001f;
	static const int numSteps = 5;
	LevelCollisions levelCollisions( *GetLevel() );

	D3DXVECTOR3 start = GetActorPosition();
	D3DXVECTOR3 end = start + m_velocity * deltaTime;
	D3DXVECTOR3 offset = end - start;
	D3DXVECTOR3 direction; D3DXVec3Normalize( &direction, &m_velocity );
	float moveLen = D3DXVec3Length( &offset );

	int step = 0;

	do
	{
		CollisionResult moveCollision;

		if( !levelCollisions.GetSphereSweep( moveCollision, start, end, m_radius ) )
		{
			break;
		}

		D3DXVECTOR3 colOffset = end - start;
		float consumedMoveLen = D3DXVec3Dot( &direction, &colOffset );
		moveLen -= consumedMoveLen;

		float collisionDot = D3DXVec3Dot( &moveCollision.m_normal, &m_velocity );

		// Check if should change direction.
		if( collisionDot < 0.0f )
		{
			float prevSpeed = D3DXVec3Length( &m_velocity );
			m_velocity -= moveCollision.m_normal * collisionDot * ( 1.0f + m_bounceFactor );
			float bounceSpeed = D3DXVec3Length( &m_velocity );
			//moveLen *= bounceSpeed / prevSpeed;
		}

		if( ++step >= numSteps || moveLen <= minimalMoveLen )
		{
			break;
		}

		// Update movement direction for next step.
		start = end;
		D3DXVec3Normalize( &direction, &m_velocity );
		offset = direction * moveLen;
		end = start + offset;

	} while( true );


	SetActorPosition( end );


	static const float touchesGroundCheckDepth = 0.01f;

	CollisionResult groundCollision;
	ShapeSphere groundSphere;
	groundSphere.m_center = GetActorPosition() - Math::s_upVector3 * touchesGroundCheckDepth * 2.0f;
	groundSphere.m_radius = m_radius - touchesGroundCheckDepth;

	m_touchesGround = levelCollisions.GetSphereOverlap( groundCollision, groundSphere ) && D3DXVec3Dot( &m_velocity, &groundCollision.m_normal ) <= 0.0f ;
}

void PhysicsCharacter::UpdateMovement( float deltaTime )
{
	D3DXVECTOR3 velocity = GetVelocity();
	// Extract y (up/down) velocity to be calculated separately.
	float velocityY = velocity.y;
	velocity.y = 0.0f;


	// Calculate 2D velocity (x, z).

	const float controlFactor = GetTouchesGround() ? 1.0f : m_moveAirControl;

	if( m_movementInput == D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )
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
		D3DXVECTOR3 velocityChangeInput = m_movementInput;

		float speedInput = D3DXVec3Length( &velocityChangeInput );

		D3DXVec3Normalize( &velocityChangeInput, &velocityChangeInput );
		D3DXVECTOR3 velocityPendicularInput = Math::GetPerpendicularVector2d( velocityChangeInput );

		float forwardSpeed = D3DXVec3Dot( &velocity, &velocityChangeInput );
		float sideSpeed = D3DXVec3Dot( &velocity, &velocityPendicularInput );

		forwardSpeed += speedInput * m_moveAcceleration * controlFactor * deltaTime;
		sideSpeed = Math::InterpolateTo( sideSpeed, 0.0f, deltaTime, m_moveDeceleration * controlFactor );

		velocity = velocityChangeInput * forwardSpeed + velocityPendicularInput * sideSpeed;


		if( m_moveSpeedMax > 0.0f && D3DXVec3Length( &velocity ) > m_moveSpeedMax )
		{
			D3DXVec3Normalize( &velocity, &velocity );
			velocity *= m_moveSpeedMax;
		}
	}

	velocityY += velocity.y;


	// Calculate up/down velocity (y).

	if( GetTouchesGround() && velocityY < 0.0f )
	{
		velocityY = 0.0f;
	}

	if( GetTouchesGround() && m_jumpInput )
	{
		velocityY += m_jumpSpeed;
	}
	else
	{
		velocityY -= m_jumpGravity * deltaTime;
	}


	velocity.y = velocityY;
	SetVelocity( velocity );


	// Clear inputs.
	m_movementInput = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_jumpInput = false;
}
