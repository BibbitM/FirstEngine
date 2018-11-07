#include "Character.h"
#include "Level.h"
#include "Terrain.h"

Character::Character()
	: m_velocity( 0.0f, 0.0f, 0.0f )
	, m_touchesGround( false ) 
{
}

Character::~Character() = default;

void Character::OnUpdate( float deltaTime )
{
	Pawn::OnUpdate( deltaTime );

	D3DXVECTOR3 position = GetActorPosition();
	position += m_velocity * deltaTime;

	const float terrainHeight = GetLevel()->GetTerrain()->GetHeight( position );


	if( position.y <= terrainHeight )
	{
		position.y = terrainHeight;
		m_touchesGround = true;
	}
	else
	{
		m_touchesGround = false;
	}

	SetActorPosition( position );
}
