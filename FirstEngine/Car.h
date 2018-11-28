#pragma once

#include "Character.h"

class GenericCamera;

class Car : public Character
{
public:
	Car();
	~Car();

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

	virtual void OnUpdate( float deltaTime ) override;

private:
	struct Input
	{
		float moveForward;
		float turnRight;
	};

	void UpdateCar( float deltaTime );
	Input GetInput() const;
	void UpdateMovement( float moveForwardInput, float deltaTime );
	void UpdateRotation( float rotationInput, float deltaTime );

	GenericCamera* m_camera;

	float m_moveAcceleration;
	float m_moveDeceleration;
	float m_moveSideDeceleration;
	float m_moveInAirDeceleration;
	float m_moveSpeedMax;

	float m_gravity;

	float m_rotationChangeSpeed;
};