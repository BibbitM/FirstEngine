#include "Projectile.h"
#include "CollisionResult.h"
#include "Collisions.h"
#include "Level.h"
#include "Math.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include "Terrain.h"
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

bool Projectile::GetNearestCollision( CollisionResult& result ) const
{
	ShapeSphere mySphere = GetCollisionSphere();

	// Collision with ground.
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	// TODO: Check collision with terrain
	// - for all relevant terrain cells check collision
	// - select collision with closer distance

	return Collisions::CheckCollisonSpherePlane( result, mySphere, ground );
}
