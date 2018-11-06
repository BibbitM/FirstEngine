#pragma once

#include "Character.h"

class Camera;

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
	};

	void UpdateTiger( float deltaTime );
	Input GetInput() const;
	void UpdateMovement( float moveForward, float moveRight, float deltaTime );
	void UpdateRotation( float rotation, float deltaTime );

	Camera* m_camera;

	float m_moveAcceleration;
	float m_moveDeceleration;
	float m_moveSpeedMax;

	float m_rotationChangeSpeed;
};