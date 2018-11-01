#pragma once

struct D3DXMATRIX;
struct D3DXVECTOR3;

namespace Math
{
	float Rad2Deg( float rad );
	float Deg2Rad( float deg );

	template< class T >
	constexpr const T& Clamp( const T& val, const T& min, const T& max )
	{
		return ( max < val ) ? max : ( val < min ) ? min : val;
	}

	D3DXMATRIX BuildMatrix( const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale );
}
