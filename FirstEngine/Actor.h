#pragma once

#include "Object.h"
#include <d3dx9math.h>

class Actor : public Object
{
public:
	Actor( const D3DXVECTOR3& position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), const D3DXVECTOR3& rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), const D3DXVECTOR3& scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	~Actor();

	const D3DXVECTOR3& GetPosition() const { return m_position; }
	const D3DXVECTOR3& GetRotation() const { return m_rotation; }
	const D3DXVECTOR3& GetScale() const { return m_scale; }

	D3DXMATRIX GetWorldMatrix() const;

	void SetPosition( const D3DXVECTOR3& position ) { m_position = position; }
	void SetRotation( const D3DXVECTOR3& rotation ) { m_rotation = rotation; }
	void SetScale( const D3DXVECTOR3& scale ) { m_scale = scale; }

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;
};