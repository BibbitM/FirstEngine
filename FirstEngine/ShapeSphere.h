#pragma once

#include <d3dx9math.h>

class ShapeSphere
{
public:
	ShapeSphere()
		: m_center( 0.0f, 0.0f, 0.0f )
		, m_radius( 0.0f )
	{ }

	D3DXVECTOR3 m_center;
	float m_radius;
};
