#pragma once

#include "NonCopyable.h"

class Level;

class NavigationManager : public NonCopyable
{
public:
	NavigationManager();
	~NavigationManager();

	void StartUp( Level* level );
	void ShutDown();

protected:
	Level* m_level;
};
