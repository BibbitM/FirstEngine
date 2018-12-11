#include "Collisions.h"
#include "CollisionResult.h"
#include "Math.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include <d3dx9math.h>

bool Collisions::LineTracePlane( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapePlane& plane )
{
	D3DXVECTOR3 direction = end - start;
	float length = D3DXVec3Length( &direction );
	D3DXVec3Normalize( &direction, &direction );

	// Line is parallel to plane.
	if( D3DXVec3Dot( &plane.m_normal, &direction ) == 0.0 )
	{
		return false;
	}

	float t = ( D3DXVec3Dot( &plane.m_normal, &plane.m_point ) - D3DXVec3Dot( &plane.m_normal, &start ) ) / D3DXVec3Dot( &plane.m_normal, &direction );

	// Collision point is to far away.
	if( t > length )
	{
		return false;
	}

	// Collision point is behind the trace.
	if( t < 0.0f )
	{
		return false;
	}

	// Fill the result.
	result.m_position = start + direction * t;
	result.m_normal = plane.m_normal;

	return true;
}

bool Collisions::LineTraceSphere( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeSphere& sphere )
{
	D3DXVECTOR3 direction = end - start;
	float length = D3DXVec3Length( &direction );
	D3DXVec3Normalize( &direction, &direction );

	D3DXVECTOR3 toCenter = sphere.m_center - start;
	float toCenterLenghtSq = D3DXVec3LengthSq( &toCenter );

	float t = D3DXVec3Dot( &direction, &toCenter );
	float xSq = toCenterLenghtSq - Math::Square( t );
	float radiusSq = Math::Square( sphere.m_radius );

	// Collision point is to far to sphere center.
	if( xSq > radiusSq )
	{
		return false;
	}

	float dt = sqrtf( radiusSq - xSq );
	float t0 = t - dt;

	// Collision point is to far away.
	if( t0 > length )
	{
		return false;
	}

	// Collision point is behind the trace.
	if( t0 < 0.0f )
	{
		return false;
	}

	// Fill the result.
	result.m_position = start + direction * t0;
	result.m_normal = result.m_position - sphere.m_center;

	return true;
}

bool Collisions::LineTraceAabb( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeAabb& aabb )
{
	// TODO:
	return false;
}

bool Collisions::CheckCollisonSphereSphere( CollisionResult& result, const ShapeSphere& mySphere, const ShapeSphere& otherAabb )
{
	// TODO:
	return false;
}

bool Collisions::CheckCollisonSpherePlane( CollisionResult& result, const ShapeSphere& mySphere, const ShapePlane& otherPlane )
{
	D3DXVECTOR3 planeToSphere = mySphere.m_center - otherPlane.m_point;

	float distToPlane = D3DXVec3Dot( &planeToSphere, &otherPlane.m_normal );

	if( distToPlane > mySphere.m_radius )
	{
		return false;
	}

	result.m_normal = otherPlane.m_normal;
	result.m_position = mySphere.m_center - otherPlane.m_normal * distToPlane;

	return true;
}

bool Collisions::CheckCollisonSphereAabb( CollisionResult& result, const ShapeSphere& mySphere, const ShapeAabb& otherAabb )
{
	// TODO:
	return false;
}
