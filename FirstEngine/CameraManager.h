#pragma once

#include "NonCopyable.h"
#include <d3dx9math.h>

class IRenderer;
class Level;

class CameraManager : public NonCopyable
{
public:
	CameraManager();
	~CameraManager();

	const D3DXVECTOR3& GetPosition() const { return m_eyePosition; }
	const D3DXVECTOR3& GetDirection() const { return m_lookDirection; }
	void SetPositionDirection( const D3DXVECTOR3& position, const D3DXVECTOR3& direction );

	int GetViewportWidth() const { return m_width; }
	int GetViewportHeight() const { return m_height; }
	float GetAspectRatio() const { return m_aspectRatio; }

	float GetFov() const { return m_fov; }
	void SetFov( float fov ) { m_fov = fov; }
	float GetNear() const { return m_near; }
	void SetNear( float near_ ) { m_near = near_; }
	float GetFar() const { return m_far; }
	void SetFar( float far_ ) { m_far = far_; }

	D3DXVECTOR3 ScreenToDirection( int x, int y );

	void StartUp( Level* level );
	void ShutDown();

	void Render() const;

protected:
	Level* m_level;

	int m_width;
	int m_height;
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;
	D3DXVECTOR3 m_eyePosition;
	D3DXVECTOR3 m_lookDirection;
};

