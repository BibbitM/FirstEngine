#include "TestCollisionShape.h"
#include "Collisions.h"
#include "ShapeAabb.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include <cassert>

TestCollisionShape::TestCollisionShape()
	: m_shape( Collisions::EShape::Sphere )
{
	SetMesh( "Content\\unitsphere.x" );
	SetColor( 0xFFAAAACC );
}

TestCollisionShape::~TestCollisionShape() = default;

void TestCollisionShape::SetShape( Collisions::EShape shape )
{
	m_shape = shape;

	switch( m_shape )
	{
	case Collisions::EShape::Plane:
		SetMesh( "Content\\plane.x" );
		break;
	case Collisions::EShape::Sphere:
		SetMesh( "Content\\unitsphere.x" );
		break;
	case Collisions::EShape::Aabb:
		SetMesh( "Content\\unitbox.x" );
		break;
	}
}

ShapePlane TestCollisionShape::GetCollisionPlane() const
{
	assert( m_shape == Collisions::EShape::Plane );

	ShapePlane plane;
	plane.m_point = GetActorWorldPosition();
	plane.m_normal = GetActorWorldUpVector();
	return plane;
}

ShapeSphere TestCollisionShape::GetCollisionSphere() const
{
	assert( m_shape == Collisions::EShape::Sphere );

	ShapeSphere sphere;
	sphere.m_center = GetActorWorldPosition();
	sphere.m_radius = 50.0f * GetActorScale().x;
	return sphere;
}

ShapeAabb TestCollisionShape::GetCollisionAabb() const
{
	assert( m_shape == Collisions::EShape::Aabb );

	ShapeAabb aabb;
	aabb.m_origin = GetActorWorldPosition();
	aabb.m_halfSize = 50.0f * GetActorScale();
	return aabb;
}
