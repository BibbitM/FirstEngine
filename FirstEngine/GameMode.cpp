#include "GameMode.h"
#include "Level.h"
#include "PacMan.h"
#include <cassert>

GameMode::GameMode()
	: m_player( nullptr )
{
}

GameMode::~GameMode()
{
	assert( !m_player );
}

void GameMode::OnStartUp()
{
	Object::OnStartUp();

	assert( !m_player );
	m_player = GetLevel()->CreateObject< PacMan >();
}

void GameMode::OnShutDown()
{
	assert( m_player );
	m_player->Destroy();
	m_player = nullptr;

	Object::OnShutDown();
}
