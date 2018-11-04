#pragma once

#include "Pawn.h"

class TestCameraPoint : public Pawn
{
public:
	TestCameraPoint();
	~TestCameraPoint();

	virtual void OnUpdate( float deltaTime ) override;
};