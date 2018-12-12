#include "TestCameraPoint.h"
#include "CameraManager.h"
#include "CollisionResult.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"
#include "LevelCollisions.h"

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
	return LevelCollisions( *GetLevel() ).GetLineTrace( collision, start, end );
}
