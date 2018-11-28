#pragma once

#include "Character.h"

class GenericCamera;

class Tiger : public Character
{
public:
	Tiger();
	~Tiger();

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

	virtual void OnUpdate( float deltaTime ) override;

private:
	struct Input
	{
		float moveForward;
		float moveRight;
		float rotation;
		bool jump;
	};

	void UpdateTiger( float deltaTime );
	Input GetInput() const;
	void UpdateMovement( float moveForwardInput, float moveRightInput, bool jumpInput, float deltaTime );
	void UpdateRotation( float rotationInput, float deltaTime );

	GenericCamera* m_camera;

	float m_moveAcceleration;
	float m_moveDeceleration;
	float m_moveSpeedMax;
	float m_moveAirControl;

	float m_jumpSpeed;
	float m_jumpGravity;

	float m_rotationChangeSpeed;
	float m_rotationAirControl;
};