#include "Character.h"

Character::Character()
	: m_velocity( 0.0f, 0.0f, 0.0f )
{
}

Character::~Character() = default;

void Character::OnUpdate( float deltaTime )
{
	Pawn::OnUpdate( deltaTime );

	D3DXVECTOR3 position = GetActorPosition();
	position += m_velocity * deltaTime;
	SetActorPosition( position );
}
