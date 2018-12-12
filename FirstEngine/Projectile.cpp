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

class Projectile::NearestCollision
{
public:
	NearestCollision( CollisionResult& collision )
		: m_collision( collision )
		, m_nearestCollisionDistanceSq( FLT_MAX )
		, m_result( false )
	{ }

	bool GetResult() const { return m_result; }

	void UpdateCollision( const CollisionResult& otherCollision, const D3DXVECTOR3& center )
	{
		m_result |= true;

		D3DXVECTOR3 collisionOffset = otherCollision.m_position - center;
		float collisionDistanceSq = D3DXVec3LengthSq( &collisionOffset );
		if( collisionDistanceSq < m_nearestCollisionDistanceSq )
		{
			m_nearestCollisionDistanceSq = collisionDistanceSq;
			m_collision = otherCollision;
		}
	}

private:
	CollisionResult& m_collision;
	float m_nearestCollisionDistanceSq;
	bool m_result;

};


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

	NearestCollision nearestCollision( collision );

	CollisionResult groundCollision;
	if( GetCollisionWithGround( groundCollision, mySphere ) )
	{
		nearestCollision.UpdateCollision( groundCollision, mySphere.m_center );
	}

	CollisionResult terrainCollision;
	if( GetCollisionWithTerrain( terrainCollision, mySphere ) )
	{
		nearestCollision.UpdateCollision( terrainCollision, mySphere.m_center );
	}

	CollisionResult testShapesCollision;
	if( GetNearestCollisionWithTestShapes( testShapesCollision, mySphere ) )
	{
		nearestCollision.UpdateCollision( testShapesCollision, mySphere.m_center );
	}

	return nearestCollision.GetResult();
}

bool Projectile::GetCollisionWithGround( CollisionResult& collision, const ShapeSphere& mySphere ) const
{
	// Ground shape.
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	return Collisions::CheckCollisonSpherePlane( collision, mySphere, ground );
}

bool Projectile::GetCollisionWithTerrain( CollisionResult& collision, const ShapeSphere& mySphere ) const
{
	NearestCollision nearestCollision( collision );


	const Terrain* terrain = GetLevel()->GetTerrain();

	D3DXVECTOR3 start = mySphere.m_center - D3DXVECTOR3( mySphere.m_radius, mySphere.m_radius, mySphere.m_radius );
	D3DXVECTOR3 end = mySphere.m_center + D3DXVECTOR3( mySphere.m_radius, mySphere.m_radius, mySphere.m_radius );

	int startX = Math::Clamp( terrain->GetX( start.x ), 0, terrain->GetCount() - 1 );
	int startZ = Math::Clamp( terrain->GetZ( start.z ), 0, terrain->GetCount() - 1 );

	int endX = Math::Clamp( terrain->GetX( end.x ), 0, terrain->GetCount() - 1 );
	int endZ = Math::Clamp( terrain->GetZ( end.z ), 0, terrain->GetCount() - 1 );

	if( startX > endX )
	{
		std::swap( startX, endX );
	}
	if( startZ > endZ )
	{
		std::swap( startZ, endZ );
	}


	CollisionResult terrainCollision;

	for( int x = startX; x <= endX; ++x )
	{
		for( int z = startZ; z <= endZ; ++z )
		{
			ShapeAabb aabb = terrain->GetCollisionShape( x, z );

			if( Collisions::CheckCollisonSphereAabb( terrainCollision, mySphere, aabb ) )
			{
				nearestCollision.UpdateCollision( terrainCollision, mySphere.m_center );
			}
		}
	}


	return nearestCollision.GetResult();
}

bool Projectile::GetNearestCollisionWithTestShapes( CollisionResult& collision, const ShapeSphere& mySphere ) const
{
	NearestCollision nearestCollision( collision );

	std::vector< TestCollisionShape* > collisionShapes = GetLevel()->GetObjectsFromClass< TestCollisionShape >();

	for( auto shape : collisionShapes )
	{
		CollisionResult shapeCollision;
		if( GetCollisionWithTestShape( shapeCollision, mySphere, *shape ) )
		{
			nearestCollision.UpdateCollision( shapeCollision, mySphere.m_center );
		}
	}

	return nearestCollision.GetResult();
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
