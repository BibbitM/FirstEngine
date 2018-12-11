#include "TestCameraPoint.h"
#include "CameraManager.h"
#include "Collisions.h"
#include "CollisionResult.h"
#include "Game.h"
#include "InputManager.h"
#include "Math.h"
#include "Level.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include "Terrain.h"
#include "TestCollisionShape.h"

TestCameraPoint::TestCameraPoint()
{
	SetMesh( "Content\\unitsphere.x" );
	SetMeshScale( D3DXVECTOR3( 0.1f, 0.1f, 0.1f ) );
}

TestCameraPoint::~TestCameraPoint() = default;

void TestCameraPoint::OnUpdate( float deltaTime )
{
	const InputManager* input = GetLevel()->GetGame()->GetInputManager();

	const int mousePosX = input->GetMousePositionX();
	const int mousePosY = input->GetMousePositionY();

	const CameraManager* camera = GetLevel()->GetCameraManager();

	D3DXVECTOR3 cameraDir = camera->ScreenToDirection( mousePosX, mousePosY );
	D3DXVECTOR3 cameraPos = camera->GetPosition();

	CollisionResult collision;

	D3DXVECTOR3 collisionPos( 0.0f, 0.0f, 0.f );

	if( GetNearestCollision( collision, cameraPos, cameraPos + cameraDir * 1000.0f ) )
	{
		collisionPos = collision.m_position;

		SetColor( 0xFF11CC11 );
	}
	else
	{
		SetColor( 0xFFCC1111 );
	}

	SetActorPosition( collisionPos );


	Pawn::OnUpdate( deltaTime );
}

bool TestCameraPoint::GetNearestCollision( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;

	if( GetCollisionWithTerrain( collision, start, testEnd ) )
	{
		result |= true;
		testEnd = collision.m_position;
	}

	if( GetNearestCollisionWithTestShapes( collision, start, testEnd ) )
	{
		result |= true;
		testEnd = collision.m_position;
	}

	return result;
}

bool TestCameraPoint::GetCollisionWithTerrain( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;


	const Terrain* terrain = GetLevel()->GetTerrain();

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

bool TestCameraPoint::GetNearestCollisionWithTestShapes( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	D3DXVECTOR3 testEnd = end;
	bool result = false;

	std::vector< TestCollisionShape* > collisionShapes = GetLevel()->GetObjectsFromClass< TestCollisionShape >();

	for( auto shape : collisionShapes )
	{
		if( GetCollisionWithTestShape( collision, start, testEnd, *shape ) )
		{
			result |= true;
			testEnd = collision.m_position;
		}
	}

	return result;
}

bool TestCameraPoint::GetCollisionWithTestShape( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const TestCollisionShape& shape ) const
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
