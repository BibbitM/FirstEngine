#pragma once

#include "Character.h"

class CollisionResult;
class ShapeSphere;

class Projectile : public Character
{
public:
	Projectile();
	~Projectile();

	float GetRadius() { return m_radius; }
	void SetRadius( float radius );

	float GetLifeTime() const { return m_lifeTime; }
	void SetLifeTime( float time ) { m_lifeTime = time; }

	float GetGravityFactor() const { return m_gravityFactor; }
	void SetGravityFactor( float factor ) { m_gravityFactor = factor; }

	ShapeSphere GetCollisionSphere() const;

protected:
	void OnUpdate( float deltaTime );

	void UpdateVelocity( float deltaTime );
	void UpdateCollision();
	void UpdateLifetTime( float deltaTime );

	bool GetNearestCollision( CollisionResult& result ) const;

private:
	float m_radius;
	float m_lifeTime;
	float m_gravityFactor;
	//float m_bounceFactor; //< TODO

	float m_lifeTimeConsumed;

	static float s_gravity;
	static float s_minimalScale;
};

