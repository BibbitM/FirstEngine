#pragma once

#include "Object.h"
#include <d3dx9math.h>

class FreeCamera : public Object
{
public:
	FreeCamera();
	~FreeCamera();

protected:
	virtual void OnUpdate( float deltaTime ) override;

private:
	struct Input
	{
		float yaw;
		float pitch;
		float moveForward;
		float moveRight;
		float moveUp;

		bool fireProjectile;
	};

	void UpdateCamera( float deltaTime );
	Input GetInput( float deltaTime ) const;
	void UpdateYaw( float yawInput );
	void UpdatePitch( float pitchInput );
	void UpdateMovement( float moveForwardInput, float moveRightInput, float moveUpInput, float deltaTime );

	void FireProjectile();

	void SetCamera();
	D3DXVECTOR3 GetCameraDirection() const;

	float m_yaw;
	float m_pitch;
	D3DXVECTOR3 m_position;

	float m_baseYawSpeed;
	float m_basePitchSpeed;
	float m_mouseYawSensitivity;
	float m_mousePitchSensitivity;
	float m_moveSpeed;
};