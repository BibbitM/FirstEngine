#include "Actor.h"
#include "Math.h"

Actor::Actor()
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_rotation( 0.0f, 0.0f, 0.0f )
	, m_scale( 1.0f, 1.0f, 1.0f )
{
}

Actor::~Actor() = default;

D3DXMATRIX Actor::GetActorWorldMatrix() const
{
	return Math::BuildMatrix( m_position, m_rotation, m_scale );
}
