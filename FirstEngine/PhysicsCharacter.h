#pragma once

#include "Character.h"

class PhysicsCharacter : public Character
{
public:
	PhysicsCharacter();
	~PhysicsCharacter();

	float GetRadius() const { return m_radius; }
	void SetRadius( float radius ) { m_radius = radius; }

	float GetBounceFactor() const { return m_bounceFactor; }
	void SetBounceFactor( float bounceFactor ) { m_bounceFactor = bounceFactor; }

protected:
	virtual void UpdateMovement( float deltaTime ) override;

private:
	float m_radius;
	float m_bounceFactor;
};
