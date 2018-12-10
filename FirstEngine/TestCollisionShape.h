#pragma once

#include "Pawn.h"

class ShapeAabb;
class ShapePlane;
class ShapeSphere;

namespace Collisions
{
	enum class EShape;
}

class TestCollisionShape : public Pawn
{
public:
	TestCollisionShape();
	~TestCollisionShape();

	Collisions::EShape GetShape() const { return m_shape; }
	void SetShape( Collisions::EShape shape );

	ShapePlane GetCollisionPlane() const;
	ShapeSphere GetCollisionSphere() const;
	ShapeAabb GetCollisionAabb() const;

private:
	Collisions::EShape m_shape;
};
