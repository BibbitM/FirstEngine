#pragma once

#include <d3dx9math.h>

class ShapePlane
{
public:
	ShapePlane()
		: m_point( 0.0f, 0.0f, 0.0f )
		, m_normal( 0.0f, 0.0f, 0.0f )
	{}

	D3DXVECTOR3 m_point;
	D3DXVECTOR3 m_normal;
};
