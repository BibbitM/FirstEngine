#include "Actor.h"
#include "Math.h"
#include <cassert>

Actor::Actor()
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_rotation( 0.0f, 0.0f, 0.0f )
	, m_scale( 1.0f, 1.0f, 1.0f )
	, m_parent( nullptr )
{
}

Actor::~Actor()
{
	assert( !m_parent );
}

D3DXMATRIX Actor::GetActorLocalMatrix() const
{
	return Math::BuildMatrix( m_position, m_rotation, m_scale );
}

D3DXMATRIX Actor::GetActorWorldMatrix() const
{
	// TODO: Calculate world matrix from parents and local.
	return GetActorLocalMatrix();
}

D3DXVECTOR3 Actor::GetActorForwardVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 forward;
	D3DXVec3TransformNormal( &forward, &Math::s_forwardVector3, &matWorld );
	return forward;
}

D3DXVECTOR3 Actor::GetActorRightVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 right;
	D3DXVec3TransformNormal( &right, &Math::s_rightVector3, &matWorld );
	return right;
}

D3DXVECTOR3 Actor::GetActorUpVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 up;
	D3DXVec3TransformNormal( &up, &Math::s_upVector3, &matWorld );
	return up;
}

bool Actor::IsAttachedTo( const Actor* parentToTest ) const
{
	UNREFERENCED_PARAMETER( parentToTest );
	// TODO: Check if this actor is attached to given parent.

	return false;
}

bool Actor::HasChild( const Actor* childToTest ) const
{
	UNREFERENCED_PARAMETER( childToTest );
	// TOOD: Check if this actor has given child.

	return false;
}

void Actor::AttachTo( Actor* parent )
{
	UNREFERENCED_PARAMETER( parent );
	// TODO: Attach to given parent.

	// If this actor is already attached to given parent than do nothing.

	// If this actor is attached to other parent then detach before proceeding.

	// Check if attaching to new parent will create cycle.

	// Do the attach.
}

void Actor::DetachFromParent()
{
	// TODO: Detach from parent.
}

void Actor::OnShutDown()
{
	// TODO: If attached to anybody then detach.

	// Detach from parent.

	// Detach children.

	Object::OnShutDown();
}
