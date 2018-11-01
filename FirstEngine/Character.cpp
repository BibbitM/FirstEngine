#include "Character.h"

Character::Character( const D3DXVECTOR3& velocity )
	: m_velocity(velocity)
{
}

Character::~Character() = default;

void Character::Update( float deltaTime )
{
	Pawn::Update( deltaTime );

	D3DXVECTOR3 position = GetActorPosition();
	position += m_velocity * deltaTime;
	SetActorPosition( position );
}
