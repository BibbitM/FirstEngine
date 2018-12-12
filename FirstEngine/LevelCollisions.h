#pragma once

class CollisionResult;
class Level;
class TestCollisionShape;

struct D3DXVECTOR3;

/** Helper class which allows check collisions with objects on given level. */
class LevelCollisions
{
public:
	LevelCollisions( const Level& level );
	~LevelCollisions();

	/// Check sphere overlap with all colliding objects on the level.
	bool GetLineTrace( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithGround( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTerrain( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTestShapes( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTestShape( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const TestCollisionShape& shape ) const;

private:
	const Level& m_level;
};
