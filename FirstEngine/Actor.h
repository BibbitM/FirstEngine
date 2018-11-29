#pragma once

#include "Object.h"
#include <d3dx9math.h>
#include <vector>

class Actor : public Object
{
public:
	Actor();
	~Actor();

	const D3DXVECTOR3& GetActorPosition() const { return m_position; }
	const D3DXVECTOR3& GetActorRotation() const { return m_rotation; }
	const D3DXVECTOR3& GetActorScale() const { return m_scale; }

	D3DXMATRIX GetActorLocalMatrix() const;
	D3DXMATRIX GetActorWorldMatrix() const;

	D3DXVECTOR3 GetActorWorldForwardVector() const;
	D3DXVECTOR3 GetActorWorldRightVector() const;
	D3DXVECTOR3 GetActorWorldUpVector() const;

	D3DXVECTOR3 GetActorWorldPosition() const;

	void SetActorPosition( const D3DXVECTOR3& position ) { m_position = position; }
	void SetActorRotation( const D3DXVECTOR3& rotation ) { m_rotation = rotation; }
	void SetActorScale( const D3DXVECTOR3& scale ) { m_scale = scale; }

	Actor* GetParent() const { return m_parent; }
	const std::vector< Actor* >& GetChildren() const { return m_children; }

	bool IsAttached() const { return m_parent != nullptr; }
	bool IsAttachedTo( const Actor* parentToTest ) const;
	bool HasChildren() const { return !m_children.empty(); }
	bool HasChild( const Actor* childToTest ) const;

	void AttachTo( Actor* parent );
	void DetachFromParent();

protected:
	virtual void OnShutDown();

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;

	Actor* m_parent;
	std::vector< Actor* > m_children;
};
