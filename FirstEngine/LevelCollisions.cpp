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
#include <d3dx9math.h>

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
