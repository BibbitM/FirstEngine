#include "Object.h"
#include "Level.h"
#include <cassert>

Object::Object()
	: m_level( nullptr )
{
}

Object::~Object()
{
	assert( !m_level );
}

void Object::StartUp( Level* level )
{
	assert( level );
	assert( !m_level );

	m_level = level;

	m_level->RegisterObject( this );

	OnStartUp();
}

void Object::ShutDown()
{
	assert( m_level );

	OnShutDown();

	m_level->UnregisterObject( this );

	m_level = nullptr;
}

void Object::Destroy()
{
	ShutDown();
	delete this;
}

void Object::Update( float deltaTime )
{
	assert( IsInitialized() );
	OnUpdate( deltaTime );
}

void Object::Render( FrameRenderer& frame ) const
{
	assert( IsInitialized() );
	OnRender( frame );
}

Level* Object::GetLevel() const
{
	assert( m_level );
	return m_level;
}

void Object::OnStartUp()
{
	// Initialize resources in sub-classes.
}

void Object::OnShutDown()
{
	// Uninitialize resources in sub-classes.
}

void Object::OnUpdate( float /*deltaTime*/ )
{
	// Default do nothing.
}

void Object::OnRender( FrameRenderer& /*frame*/ ) const
{
	// Default do not render.
}
