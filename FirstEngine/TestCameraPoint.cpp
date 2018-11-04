#include "TestCameraPoint.h"
#include "CameraManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Level.h"

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

	D3DXVECTOR3 groundPos( 0.0f, 0.0f, 0.0f );

	if( cameraDir.y < -0.01f && cameraPos.y > 0.0f ||
		cameraDir.y > +0.01f && cameraPos.y < 0.0f )
	{
		groundPos = cameraPos - cameraDir * ( cameraPos.y / cameraDir.y );
		groundPos.y = 0.0f;

		SetColor( 0xFF11CC11 );
	}
	else
	{
		SetColor( 0xFFCC1111 );
	}

	SetActorPosition( groundPos );


	Pawn::OnUpdate( deltaTime );
}
