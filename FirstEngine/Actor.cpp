#include "Actor.h"
#include "Math.h"
#include <algorithm>
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
	if( IsAttached() )
	{
		return GetActorLocalMatrix() * GetParent()->GetActorWorldMatrix();
	}
	else
	{
		return GetActorLocalMatrix();
	}
}

D3DXVECTOR3 Actor::GetActorWorldForwardVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 forward;
	D3DXVec3TransformNormal( &forward, &Math::s_forwardVector3, &matWorld );
	return forward;
}

D3DXVECTOR3 Actor::GetActorWorldRightVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 right;
	D3DXVec3TransformNormal( &right, &Math::s_rightVector3, &matWorld );
	return right;
}

D3DXVECTOR3 Actor::GetActorWorldUpVector() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 up;
	D3DXVec3TransformNormal( &up, &Math::s_upVector3, &matWorld );
	return up;
}

D3DXVECTOR3 Actor::GetActorWorldPosition() const
{
	D3DXMATRIX matWorld = GetActorWorldMatrix();
	D3DXVECTOR3 worldPos;
	D3DXVec3TransformCoord( &worldPos, &Math::s_zeroVector3, &matWorld );
	return worldPos;
}

bool Actor::IsAttachedTo( const Actor* parentToTest ) const
{
	if( !parentToTest )
	{
		return false;
	}

	for( Actor* parent = m_parent; parent != nullptr; parent = parent->GetParent() )
	{
		if( parentToTest == parent )
		{
			return true;
		}
	}

	return false;
}

bool Actor::HasChild( const Actor* childToTest ) const
{
	for( const Actor* child : m_children )
	{
		if( child == childToTest )
		{
			return true;
		}
	}

	return false;
}

void Actor::AttachTo( Actor* parent )
{
	assert( parent );

	if( m_parent == parent )
	{
		assert( m_parent->HasChild( this ) );
		return;
	}

	if( m_parent )
	{
		DetachFromParent();
	}

	// Check if attaching to new parent will create cycle.
	if( parent->IsAttachedTo( this ) )
	{
		return;
	}

	m_parent = parent;
	m_parent->m_children.push_back( this );
}

void Actor::DetachFromParent()
{
	assert( m_parent );
	assert( m_parent->HasChild( this ) );

	m_parent->m_children.erase( std::remove( m_parent->m_children.begin(), m_parent->m_children.end(), this ), m_parent->m_children.end() );
	m_parent = nullptr;
}

void Actor::OnShutDown()
{
	if( IsAttached() )
	{
		DetachFromParent();
	}

	while( HasChildren() )
	{
		m_children[ 0 ]->DetachFromParent();
	}

	Object::OnShutDown();
}
