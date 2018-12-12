#include "LevelCollisions.h"
#include "CollisionResult.h"
#include "Collisions.h"
#include "Math.h"
#include "Level.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include "Terrain.h"
#include "TestCollisionShape.h"
#include <algorithm>
#include <cassert>
#include <d3dx9math.h>

// Fix <windows.h> defines
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


class LevelCollisions::NearestCollision
{
public:
	NearestCollision( CollisionResult& collision )
		: m_collision( collision )
		, m_nearestCollisionDistanceSq( FLT_MAX )
		, m_result( false )
	{}

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


LevelCollisions::LevelCollisions( const Level& level )
	: m_level( level )
{
}

LevelCollisions::~LevelCollisions() = default;


bool LevelCollisions::GetLineTrace( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;

	if( GetLineTraceWithGround( collision, start, testEnd ) )
	{
		result |= true;
		testEnd = collision.m_position;
	}

	if( GetLineTraceWithTerrain( collision, start, testEnd ) )
	{
		result |= true;
		testEnd = collision.m_position;
	}

	if( GetLineTraceWithTestShapes( collision, start, testEnd ) )
	{
		result |= true;
		testEnd = collision.m_position;
	}

	return result;
}

bool LevelCollisions::GetLineTraceWithGround( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	// Ground shape.
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	return Collisions::LineTracePlane( collision, start, end, ground );
}

bool LevelCollisions::GetLineTraceWithTerrain( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;


	const Terrain* terrain = m_level.GetTerrain();

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

	for( int x = startX; x <= endX; ++x )
	{
		for( int z = startZ; z <= endZ; ++z )
		{
			ShapeAabb aabb = terrain->GetCollisionShape( x, z );

			if( Collisions::LineTraceAabb( collision, start, testEnd, aabb ) )
			{
				result |= true;
				testEnd = collision.m_position;
			}
		}
	}


	return result;
}

bool LevelCollisions::GetLineTraceWithTestShapes( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;

	std::vector< TestCollisionShape* > collisionShapes = m_level.GetObjectsFromClass< TestCollisionShape >();

	for( auto shape : collisionShapes )
	{
		if( GetLineTraceWithTestShape( collision, start, testEnd, *shape ) )
		{
			result |= true;
			testEnd = collision.m_position;
		}
	}

	return result;
}

bool LevelCollisions::GetLineTraceWithTestShape( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const TestCollisionShape& shape ) const
{
	switch( shape.GetShape() )
	{
	case Collisions::EShape::Plane:
		return Collisions::LineTracePlane( collision, start, end, shape.GetCollisionPlane() );
	case Collisions::EShape::Sphere:
		return Collisions::LineTraceSphere( collision, start, end, shape.GetCollisionSphere() );
	case Collisions::EShape::Aabb:
		return Collisions::LineTraceAabb( collision, start, end, shape.GetCollisionAabb() );
	default:
		return false;
	}
}

bool LevelCollisions::GetSphereOverlap( CollisionResult& collision, const ShapeSphere& sphere ) const
{
	NearestCollision nearestCollision( collision );

	CollisionResult groundCollision;
	if( GetSphereOverlapWithGround( groundCollision, sphere ) )
	{
		nearestCollision.UpdateCollision( groundCollision, sphere.m_center );
	}

	CollisionResult terrainCollision;
	if( GetSphereOverlapWithTerrain( terrainCollision, sphere ) )
	{
		nearestCollision.UpdateCollision( terrainCollision, sphere.m_center );
	}

	CollisionResult testShapesCollision;
	if( GetSphereOverlapWithTestShapes( testShapesCollision, sphere ) )
	{
		nearestCollision.UpdateCollision( testShapesCollision, sphere.m_center );
	}

	return nearestCollision.GetResult();
}

bool LevelCollisions::GetSphereOverlapWithGround( CollisionResult& collision, const ShapeSphere& sphere ) const
{
	// Ground shape.
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	return Collisions::CheckCollisonSpherePlane( collision, sphere, ground );
}

bool LevelCollisions::GetSphereOverlapWithTerrain( CollisionResult& collision, const ShapeSphere& sphere ) const
{
	NearestCollision nearestCollision( collision );


	const Terrain* terrain = m_level.GetTerrain();

	D3DXVECTOR3 start = sphere.m_center - D3DXVECTOR3( sphere.m_radius, sphere.m_radius, sphere.m_radius );
	D3DXVECTOR3 end = sphere.m_center + D3DXVECTOR3( sphere.m_radius, sphere.m_radius, sphere.m_radius );

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

			if( Collisions::CheckCollisonSphereAabb( terrainCollision, sphere, aabb ) )
			{
				nearestCollision.UpdateCollision( terrainCollision, sphere.m_center );
			}
		}
	}


	return nearestCollision.GetResult();
}

bool LevelCollisions::GetSphereOverlapWithTestShapes( CollisionResult& collision, const ShapeSphere& sphere ) const
{
	NearestCollision nearestCollision( collision );

	std::vector< TestCollisionShape* > collisionShapes = m_level.GetObjectsFromClass< TestCollisionShape >();

	for( auto shape : collisionShapes )
	{
		CollisionResult shapeCollision;
		if( GetSphereOverlapWithTestShape( shapeCollision, sphere, *shape ) )
		{
			nearestCollision.UpdateCollision( shapeCollision, sphere.m_center );
		}
	}

	return nearestCollision.GetResult();
}

bool LevelCollisions::GetSphereOverlapWithTestShape( CollisionResult& collision, const ShapeSphere& sphere, const TestCollisionShape& shape ) const
{
	switch( shape.GetShape() )
	{
	case Collisions::EShape::Plane:
		return Collisions::CheckCollisonSpherePlane( collision, sphere, shape.GetCollisionPlane() );
	case Collisions::EShape::Sphere:
		return Collisions::CheckCollisonSphereSphere( collision, sphere, shape.GetCollisionSphere() );
	case Collisions::EShape::Aabb:
		return Collisions::CheckCollisonSphereAabb( collision, sphere, shape.GetCollisionAabb() );
	default:
		return false;
	}
}

bool LevelCollisions::GetSphereSweep( CollisionResult& collision, const D3DXVECTOR3& start, D3DXVECTOR3& end, float sphereRadius ) const
{
	bool result = false;

	// First check with line trace.
	if( GetLineTrace( collision, start, end ) )
	{
		result |= true;
		end = collision.m_position;
	}

	// For radius is 0 do the line trace only.
	if( sphereRadius <= 0.0f )
	{
		return result;
	}


	// Check collision at end position.
	ShapeSphere sphere;
	sphere.m_center = end;
	sphere.m_radius = sphereRadius;

	if( !GetSphereOverlap( collision, sphere ) )
	{
		assert( !result );
		return result;
	}

	result |= true;

	static const float avoidPenetrationEpsilon = 0.001f;

	// Get the final position;
	D3DXVECTOR3 toCollision = collision.m_position - end;
	float penetration = sphereRadius - D3DXVec3Length( &toCollision ) + avoidPenetrationEpsilon;

	end += collision.m_normal * penetration;

	return result;
}
