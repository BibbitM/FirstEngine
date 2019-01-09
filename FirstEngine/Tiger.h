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
	void UpdateTiger( float deltaTime );
	float UpdateInput();
	void UpdateRotation( float rotationInput, float deltaTime );

	GenericCamera* m_camera;

	float m_rotationChangeSpeed;
	float m_rotationAirControl;
};