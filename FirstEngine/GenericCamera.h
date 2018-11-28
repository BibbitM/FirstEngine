#pragma once

#include "Object.h"
#include <d3dx9math.h>

class Actor;

class GenericCamera : public Object
{
public:
	GenericCamera();
	~GenericCamera();

	const Actor* GetTarget() const { return m_target; }

	void SetTarget( const Actor* target );

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
	void UpdateYawFromTargetMovement();
	void UpdateYaw( float yawInput );
	void UpdatePitch( float pitchInput );
	void UpdateDistance( float deltaTime, float distanceInput );
	void StoreTargetPosition();

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

	D3DXVECTOR3 m_previousTargetPosition;
	bool m_previousTargetPositionSet;
};