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

	float GetMoveAcceleration() const { return m_moveAcceleration; }
	float GetMoveDeceleration() const { return m_moveDeceleration; }
	float GetMoveSpeedMax() const { return m_moveSpeedMax; }
	float GetMoveAirControl() const { return m_moveAirControl; }
	float GetJumpSpeed() const { return m_jumpSpeed; }
	float GetJumpGravity() const { return m_jumpGravity; }

	void SetMoveAcceleration( float moveAcceleration ) { m_moveAcceleration = moveAcceleration; }
	void SetMoveDeceleration( float moveDeceleration ) { m_moveDeceleration = moveDeceleration; }
	void SetMoveSpeedMax( float moveSpeedMax ) { m_moveSpeedMax = moveSpeedMax; }
	void SetMoveAirControl( float moveAirControl ) { m_moveAirControl = moveAirControl; }
	void SetJumpSpeed( float jumpSpeed ) { m_jumpSpeed = jumpSpeed; }
	void SetJumpGravity( float jumpGravity ) { m_jumpGravity = jumpGravity; }

	void AddMovementInput( const D3DXVECTOR3 moveInput ) { m_movementInput += moveInput; }
	void AddJumpInput() { m_jumpInput = true; }

	const D3DXVECTOR3& GetMovementInput() const { return m_movementInput; }
	bool GetJumpInput() const { return m_jumpInput; }

protected:
	virtual void OnUpdate( float deltaTime ) override;

	virtual void UpdateActorPosition( float deltaTime ) override;
	void UpdateMovement( float deltaTime );

private:
	float m_radius;
	float m_bounceFactor;

	float m_moveAcceleration;
	float m_moveDeceleration;
	float m_moveSpeedMax;
	float m_moveAirControl;

	float m_jumpSpeed;
	float m_jumpGravity;

	D3DXVECTOR3 m_movementInput;
	bool m_jumpInput;
};
