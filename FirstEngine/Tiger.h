#pragma once

#include "Pawn.h"

class Camera;

class Tiger : public Pawn
{
public:
	Tiger();
	~Tiger();

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

	virtual void OnUpdate( float deltaTime ) override;

private:
	void UpdateRotation( float deltaTime );
	float GetRotationInput() const;

	Camera* m_camera;
};