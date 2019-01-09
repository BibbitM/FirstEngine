#include "GameMode.h"
#include "CollisionResult.h"
#include "Level.h"
#include "LevelCollisions.h"
#include "PacMan.h"
#include <cassert>

GameMode::GameMode()
	: m_player( nullptr )
{
}

GameMode::~GameMode()
{
	assert( !m_player );
}

void GameMode::OnStartUp()
{
	Object::OnStartUp();

	assert( !m_player );
	m_player = GetLevel()->CreateObject< PacMan >( [ this ]( auto pacMan )
	{
		LevelCollisions levelCollisions( *GetLevel() );

		CollisionResult collision;
		D3DXVECTOR3 pacManPosition = pacMan->GetActorPosition();
		if( levelCollisions.GetSphereSweep( collision, pacManPosition + D3DXVECTOR3( 0.0f, 100.0f, 0.0f ), pacManPosition, pacMan->GetRadius() ) )
		{
			pacMan->SetActorPosition( pacManPosition );
		}
	} );
}

void GameMode::OnShutDown()
{
	assert( m_player );
	m_player->Destroy();
	m_player = nullptr;

	Object::OnShutDown();
}
