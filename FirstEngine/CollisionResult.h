#pragma once

#include <d3dx9math.h>

class CollisionResult
{
public:
	CollisionResult()
		: m_position( 0.0f, 0.0f, 0.0f )
		, m_normal( 0.0f, 0.0f, 0.0f )
	{ }

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_normal;
};
