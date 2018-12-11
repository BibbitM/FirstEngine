#include "Projectile.h"
#include "CollisionResult.h"
#include "Collisions.h"
#include "Level.h"
#include "Math.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include "Terrain.h"
#include "TestCollisionShape.h"
#include <algorithm>

#ifdef max
#undef max
#endif

float Projectile::s_gravity = 10.0f;

float Projectile::s_minimalScale = 0.01f;


Projectile::Projectile()
	: m_radius( 0.0f )
	, m_lifeTime( 0.0f )
	, m_gravityFactor( 0.0f )
	, m_lifeTimeConsumed( 0.0f )
{
	SetMesh( "Content\\unitsphere.x" );
	SetColor( 0xFFFF0000 );

	float scale = std::max( m_radius, s_minimalScale );
	SetActorScale( D3DXVECTOR3( scale, scale, scale ) );
}


Projectile::~Projectile() = default;

void Projectile::SetRadius( float radius )
{
	m_radius = radius;

	float scale = std::max( m_radius, s_minimalScale );
	SetActorScale( D3DXVECTOR3( scale, scale, scale ) );
}

ShapeSphere Projectile::GetCollisionSphere() const
{
	ShapeSphere sphere;

	sphere.m_center = GetActorWorldPosition();
	sphere.m_radius = m_radius;

	return sphere;
}

void Projectile::OnUpdate( float deltaTime )
{
	UpdateVelocity( deltaTime );

	// Base class simulates movement.
	Character::OnUpdate( deltaTime );

	UpdateCollision();

	// Call it as last. It can delete this.
	UpdateLifetTime( deltaTime );
}

void Projectile::UpdateVelocity( float deltaTime )
{
	if( m_gravityFactor == 0.0f )
	{
		return;
	}

	SetVelocity( GetVelocity() - Math::s_upVector3 * s_gravity * m_gravityFactor * deltaTime );
}

void Projectile::UpdateCollision()
{
	CollisionResult collision;

	if( !GetNearestCollision( collision ) )
	{
		return;
	}


	float collisionDot = D3DXVec3Dot( &collision.m_normal, &GetVelocity() );

	// Check if can bounce.
	if( collisionDot >= 0.0f )
	{
		return;
	}

	// Bounce.
	SetVelocity( GetVelocity() - collision.m_normal * collisionDot * 2.0f );
}

void Projectile::UpdateLifetTime( float deltaTime )
{
	if( m_lifeTime <= 0.0f )
	{
		return;
	}

	m_lifeTimeConsumed += deltaTime;

	if( m_lifeTimeConsumed >= m_lifeTime )
	{
		Destroy();
	}
}

bool Projectile::GetNearestCollision( CollisionResult& collision ) const
{
	ShapeSphere mySphere = GetCollisionSphere();
	bool result = false;
	float nearestCollisionDistanceSq = FLT_MAX;

	CollisionResult groundCollision;
	if( GetCollisionWithGround( groundCollision, mySphere ) )
	{
		result |= true;

		D3DXVECTOR3 collisionOffset = groundCollision.m_position - mySphere.m_center;
		float collisionDistanceSq = D3DXVec3LengthSq( &collisionOffset );
		if( collisionDistanceSq < nearestCollisionDistanceSq )
		{
			nearestCollisionDistanceSq = collisionDistanceSq;
			collision = groundCollision;
		}
	}

	CollisionResult testShapesCollision;
	if( GetNearestCollisionWithTestShapes( testShapesCollision, mySphere ) )
	{
		result |= true;

		D3DXVECTOR3 collisionOffset = testShapesCollision.m_position - mySphere.m_center;
		float collisionDistanceSq = D3DXVec3LengthSq( &collisionOffset );
		if( collisionDistanceSq < nearestCollisionDistanceSq )
		{
			nearestCollisionDistanceSq = collisionDistanceSq;
			collision = testShapesCollision;
		}
	}

	return result;
}

bool Projectile::GetCollisionWithGround( CollisionResult& collision, const ShapeSphere& mySphere ) const
{
	// Ground shape.
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	return Collisions::CheckCollisonSpherePlane( collision, mySphere, ground );
}

bool Projectile::GetNearestCollisionWithTestShapes( CollisionResult& collision, const ShapeSphere& mySphere ) const
{
	bool result = false;
	float nearestCollisionDistanceSq = FLT_MAX;

	std::vector< TestCollisionShape* > collisionShapes = GetLevel()->GetObjectsFromClass< TestCollisionShape >();

	for( auto shape : collisionShapes )
	{
		CollisionResult shapeCollision;
		if( GetCollisionWithTestShape( shapeCollision, mySphere, *shape ) )
		{
			result |= true;

			D3DXVECTOR3 collisionOffset = shapeCollision.m_position - mySphere.m_center;
			float collisionDistanceSq = D3DXVec3LengthSq( &collisionOffset );
			if( collisionDistanceSq < nearestCollisionDistanceSq )
			{
				nearestCollisionDistanceSq = collisionDistanceSq;
				collision = shapeCollision;
			}
		}
	}

	return result;
}

bool Projectile::GetCollisionWithTestShape( CollisionResult& collision, const ShapeSphere& mySphere, const TestCollisionShape& shape ) const
{
	switch( shape.GetShape() )
	{
	case Collisions::EShape::Plane:
		return Collisions::CheckCollisonSpherePlane( collision, mySphere, shape.GetCollisionPlane() );
	case Collisions::EShape::Sphere:
		return Collisions::CheckCollisonSphereSphere( collision, mySphere, shape.GetCollisionSphere() );
	case Collisions::EShape::Aabb:
		return Collisions::CheckCollisonSphereAabb( collision, mySphere, shape.GetCollisionAabb() );
	default:
		return false;
	}
}
