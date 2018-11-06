#pragma once

#include "Pawn.h"

class Character : public Pawn
{
public:
	Character();
	~Character();

	const D3DXVECTOR3& GetVelocity() const { return m_velocity; }

	void SetVelocity( const D3DXVECTOR3& velocity ) { m_velocity = velocity; }

	bool GetTouchesGround() const { return m_touchesGround; }

protected:
	virtual void OnUpdate( float deltaTime ) override;

private:
	D3DXVECTOR3 m_velocity;

	bool m_touchesGround;

};