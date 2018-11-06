#include "Character.h"

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
	if( position.y <= 0.0f )
	{
		position.y = 0.0f;
		m_touchesGround = true;
	}
	else
	{
		m_touchesGround = false;
	}

	SetActorPosition( position );
}
