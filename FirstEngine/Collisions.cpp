#include "Collisions.h"
#include "CollisionResult.h"
#include "ShapeAabb.h"
#include "ShapeSphere.h"
#include <d3dx9math.h>

bool Collisions::LineTraceSphere( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeAabb& aabb )
{
	return false;
}

bool Collisions::LineTraceAabb( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeAabb& aabb )
{
	return false;
}

bool Collisions::CheckCollisonSphereSphere( CollisionResult& result, const ShapeSphere& mySphere, const ShapeSphere& otherAabb )
{
	return false;
}

bool Collisions::CheckCollisonSphereAabb( CollisionResult& result, const ShapeSphere& mySphere, const ShapeAabb& otherAabb )
{
	return false;
}
