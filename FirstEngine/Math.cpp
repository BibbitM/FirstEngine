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

D3DXMATRIX Math::BuildMatrix( const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale )
{
	D3DXMATRIX worldMatrix;

	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scalingMatrix;

	D3DXMatrixTranslation( &translationMatrix, position.x, position.y, position.z );

	D3DXMatrixRotationYawPitchRoll( &rotationMatrix, rotation.y, rotation.x, rotation.z );

	D3DXMatrixScaling( &scalingMatrix, scale.x, scale.y, scale.z );

	return worldMatrix = rotationMatrix * scalingMatrix * translationMatrix;
}
