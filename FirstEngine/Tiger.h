#pragma once

#include "PhysicsCharacter.h"

class GenericCamera;

class Tiger : public PhysicsCharacter
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
	Input GetInput();
	void UpdateRotation( float rotationInput, float deltaTime );

	GenericCamera* m_camera;

	float m_rotationChangeSpeed;
	float m_rotationAirControl;
};