#include "Actor.h"
#include "Math.h"

Actor::Actor( const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale )
	: m_position( position )
	, m_rotation( rotation )
	, m_scale( scale )
{
}

Actor::~Actor() = default;

D3DXMATRIX Actor::GetActorWorldMatrix() const
{
	return Math::BuildMatrix( m_position, m_rotation, m_scale );
}
