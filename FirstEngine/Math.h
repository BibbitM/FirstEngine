#pragma once

namespace Math
{
	float Rad2Deg( float rad );
	float Deg2Rad( float deg );

	template< class T >
	constexpr const T& Clamp( const T& val, const T& min, const T& max )
	{
		return ( max < val ) ? max : ( val < min ) ? min : val;
	}
}
