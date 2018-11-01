#include "Actor.h"

Actor::Actor( const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale )
	: m_position( position )
	, m_rotation( rotation )
	, m_scale( scale )
{
}

Actor::~Actor() = default;

D3DXMATRIX Actor::GetWorldMatrix() const
{
	D3DXMATRIX worldMatrix;

	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scalingMatrix;

	D3DXMatrixTranslation( &translationMatrix, m_position.x, m_position.y, m_position.z );

	D3DXMatrixRotationYawPitchRoll( &rotationMatrix, m_rotation.y, m_rotation.x, m_rotation.z );

	D3DXMatrixScaling( &scalingMatrix, m_scale.x, m_scale.y, m_scale.z );

	return worldMatrix = rotationMatrix * scalingMatrix * translationMatrix;
}
