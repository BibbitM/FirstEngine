#include "NavigationManager.h"
#include <cassert>

NavigationManager::NavigationManager()
	: m_level( nullptr )
{
}

NavigationManager::~NavigationManager()
{
	assert( !m_level );
}

void NavigationManager::StartUp( Level* level )
{
	assert( !m_level );
	m_level = level;
}

void NavigationManager::ShutDown()
{
	assert( m_level );
	m_level = nullptr;
}
