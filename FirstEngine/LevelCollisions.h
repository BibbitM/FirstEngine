#pragma once

class CollisionResult;
class Level;
class ShapeSphere;
class TestCollisionShape;

struct D3DXVECTOR3;

/** Helper class which allows check collisions with objects on given level. */
class LevelCollisions
{
public:
	LevelCollisions( const Level& level );
	~LevelCollisions();

	/// Check line trace with all colliding objects on the level.
	bool GetLineTrace( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithGround( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTerrain( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTestShapes( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end ) const;
	bool GetLineTraceWithTestShape( CollisionResult& collision, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const TestCollisionShape& shape ) const;

	/// Check sphere overlap with all colliding objects on the level.
	bool GetSphereOverlap( CollisionResult& collision, const ShapeSphere& sphere ) const;
	bool GetSphereOverlapWithGround( CollisionResult& collision, const ShapeSphere& sphere ) const;
	bool GetSphereOverlapWithTerrain( CollisionResult& collision, const ShapeSphere& sphere ) const;
	bool GetSphereOverlapWithTestShapes( CollisionResult& collision, const ShapeSphere& sphere ) const;
	bool GetSphereOverlapWithTestShape( CollisionResult& collision, const ShapeSphere& sphere, const TestCollisionShape& shape ) const;

private:
	class NearestCollision;

	const Level& m_level;
};
