#pragma once

#include "Object.h"

class Actor;
struct D3DXVECTOR3;

class Camera : public Object
{
public:
	Camera();
	~Camera();

	const Actor* GetTarget() const { return m_target; }

	void SetTarget( const Actor* target ) { m_target = target; }

	D3DXVECTOR3 GetTargetPosition() const;

protected:
	virtual void OnUpdate( float deltaTime ) override;

private:
	struct Input
	{
		float yaw;
		float pitch;
		float distance;
	};

	void UpdateCamera( float deltaTime );
	Input GetInput( float deltaTime ) const;
	void UpdateYaw( float yawInput );
	void UpdatePitch( float pitchInput );
	void UpdateDistance( float deltaTime, float distanceInput );

	void SetCamera();
	D3DXVECTOR3 GetCameraDirection() const;

	const Actor* m_target;

	float m_yaw;
	float m_pitch;
	float m_currentDistance;
	float m_wantedDistance;

	float m_baseYawSpeed;
	float m_basePitchSpeed;
	float m_mouseYawSensitivity;
	float m_mousePitchSensitivity;

	float m_distanceChangeStep;
	float m_distanceMin;
	float m_distanceMax;
	float m_distanceChangeSpeed;
};