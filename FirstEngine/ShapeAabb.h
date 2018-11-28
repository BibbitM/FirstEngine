#pragma once

#include <d3dx9math.h>

class ShapeAabb
{
public:
	ShapeAabb()
		: m_origin( 0.0f, 0.0f, 0.0f )
		, m_halfSize( 0.0f, 0.0f, 0.0f )
	{ }

	D3DXVECTOR3 m_origin;
	D3DXVECTOR3 m_halfSize;
};