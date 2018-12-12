#include "PhysicsCharacter.h"
#include "CollisionResult.h"
#include "LevelCollisions.h"
#include "Math.h"
#include "ShapeSphere.h"

PhysicsCharacter::PhysicsCharacter()
	: m_radius( 0.05f )
	, m_bounceFactor( 0.0f )
{
}

PhysicsCharacter::~PhysicsCharacter() = default;


void PhysicsCharacter::UpdateMovement( float deltaTime )
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
