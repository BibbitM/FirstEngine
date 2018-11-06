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

	template< class T >
	constexpr const T InterpolateTo( const T& current, const T& target, float deltaTime, const T& speed )
	{
		if( target > current )
		{
			return Clamp( current + speed * deltaTime, current, target );
		}
		else if( target < current )
		{
			return Clamp( current - speed * deltaTime, target, current );
		}
		else
		{
			return current;
		}
	}

	D3DXVECTOR3 InterpolateTo( const D3DXVECTOR3& current, const D3DXVECTOR3& target, float deltaTime, float speed );

	D3DXMATRIX BuildMatrix( const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale );
}
