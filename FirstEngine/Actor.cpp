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

D3DXVECTOR3 Actor::GetActorForwardVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 forward;
	D3DXVec3TransformNormal( &forward, &Math::s_forwardVector3, &matWorld );
	return forward;
}

D3DXVECTOR3 Actor::GetActorRightVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 right;
	D3DXVec3TransformNormal( &right, &Math::s_rightVector3, &matWorld );
	return right;
}

D3DXVECTOR3 Actor::GetActorUpVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 up;
	D3DXVec3TransformNormal( &up, &Math::s_upVector3, &matWorld );
	return up;
}
