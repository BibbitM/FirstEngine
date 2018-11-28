#pragma once

class CollisionResult;
class ShapeAabb;
class ShapePlane;
class ShapeSphere;

struct D3DXVECTOR3;

namespace Collisions
{
	/// Checks collision between line and plane.
	bool LineTracePlane( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapePlane& plane );
	/// Checks collision between line and sphere.
	bool LineTraceSphere( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeAabb& aabb );
	/// Checks collision between line and aabb.
	bool LineTraceAabb( CollisionResult& result, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const ShapeAabb& aabb );

	/// Checks collision between two spheres.
	bool CheckCollisonSphereSphere( CollisionResult& result, const ShapeSphere& mySphere, const ShapeSphere& otherAabb );
	/// Checks collision between sphere and plane.
	bool CheckCollisonSpherePlane( CollisionResult& result, const ShapeSphere& mySphere, const ShapePlane& otherPlane );
	/// Checks collision between sphere and aabb.
	bool CheckCollisonSphereAabb( CollisionResult& result, const ShapeSphere& mySphere, const ShapeAabb& otherAabb );
}
