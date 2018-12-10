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

	if( GetCollisionWithGround( collision, start, testEnd ) )
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

bool TestCameraPoint::GetCollisionWithGround( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const
{
	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	return Collisions::LineTracePlane( collision, start, end, ground );
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
