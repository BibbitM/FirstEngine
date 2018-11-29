#include "TestCameraPoint.h"
#include "CameraManager.h"
#include "Collisions.h"
#include "CollisionResult.h"
#include "Game.h"
#include "InputManager.h"
#include "Math.h"
#include "Level.h"
#include "ShapePlane.h"

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

	ShapePlane ground;
	ground.m_point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ground.m_normal = Math::s_upVector3;

	CollisionResult collResult;

	D3DXVECTOR3 groundPos( 0.0f, 0.0f, 0.f );

	if( Collisions::LineTracePlane( collResult, cameraPos, cameraDir * 1000.0f, ground ) )
	{
		groundPos = collResult.m_position;

		SetColor( 0xFF11CC11 );
	}
	else
	{
		SetColor( 0xFFCC1111 );
	}

	SetActorPosition( groundPos );


	Pawn::OnUpdate( deltaTime );
}
