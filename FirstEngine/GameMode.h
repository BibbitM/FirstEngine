#pragma once

#include "Object.h"

class Character;

class GameMode : public Object
{
public:
	GameMode();
	~GameMode();

	Character* GetPlayer() const { return m_player; }

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

private:
	Character* m_player;
};
