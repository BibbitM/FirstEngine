#pragma once

#include "Pawn.h"

class CollisionResult;
class TestCollisionShape;

class TestCameraPoint : public Pawn
{
public:
	TestCameraPoint();
	~TestCameraPoint();

	virtual void OnUpdate( float deltaTime ) override;

private:
	bool GetNearestCollision( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;

	bool GetCollisionWithTerrain( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetNearestCollisionWithTestShapes( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetCollisionWithTestShape( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const TestCollisionShape& shape ) const;
};
