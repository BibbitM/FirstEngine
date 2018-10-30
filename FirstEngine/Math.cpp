#include "Math.h"
#include <d3dx9math.h>

float Math::Rad2Deg( float rad )
{
	return D3DXToDegree( rad );
}

float Math::Deg2Rad( float deg )
{
	return D3DXToRadian( deg );
}
