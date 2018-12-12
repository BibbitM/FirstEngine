#pragma once

#include "Pawn.h"

class CollisionResult;

class TestCameraPoint : public Pawn
{
public:
	TestCameraPoint();
	~TestCameraPoint();

	virtual void OnUpdate( float deltaTime ) override;

private:
	bool GetNearestCollision( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
};
