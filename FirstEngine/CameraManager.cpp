#include "CameraManager.h"
#include "Game.h"
#include "Math.h"
#include "IRenderer.h"
#include "Level.h"
#include "RendererDevice.h"
#include <algorithm>
#include <cassert>
#include <d3d.h>

CameraManager::CameraManager()
	: m_level( nullptr )
	, m_width( 1 )
	, m_height( 1 )
	, m_fov( Math::Deg2Rad( 60.0f ) )
	, m_aspectRatio( 1.0f )
	, m_near( 0.1f )
	, m_far( 1000.0f )
	, m_eyePosition( 0.0f, 1.0f, 0.0f )
	, m_lookDirection( 1.0f, 0.0f, 0.0f ) //< Forward
{
}

CameraManager::~CameraManager()
{
	assert( !m_level );
}

void CameraManager::SetPositionDirection( const D3DXVECTOR3& position, const D3DXVECTOR3& direction )
{
	m_eyePosition = position;
	m_lookDirection = direction;
}

D3DXVECTOR3 CameraManager::ScreenToDirection( int screenX, int screenY ) const
{
	// Get Projection matrix
	D3DXMATRIX matProj;
	D3DXMATRIX invProj;
	D3DXMatrixPerspectiveFovLH( &matProj, m_fov, m_aspectRatio, m_near, m_far );
	D3DXMatrixInverse( &invProj, nullptr, &matProj );

	D3DXMATRIX matView;
	D3DXMATRIX invView;
	D3DXVECTOR3 lookAtPosition = m_eyePosition + m_lookDirection * 50.0f;
	D3DXVECTOR3 upVector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &m_eyePosition, &lookAtPosition, &upVector );
	D3DXMatrixInverse( &invView, nullptr, &matView );


	float px = ( ( ( 2.0f * ( screenX + 0.5f ) ) / m_width ) - 1.0f );
	float py = ( ( ( -2.0f * ( screenY + 0.5f ) ) / m_height ) + 1.0f );

	D3DXVECTOR3 rayDirection = D3DXVECTOR3( px, py, 1.0f );

	// Transform the ray's direction.
	// - Convert from screen space to view space.
	D3DXVec3TransformCoord( &rayDirection, &rayDirection, &invProj );
	// - Convert from view space to world space.
	D3DXVec3TransformNormal( &rayDirection, &rayDirection, &invView );
	// - Normalize the direction
	D3DXVec3Normalize( &rayDirection, &rayDirection );

	return rayDirection;
}

void CameraManager::StartUp( Level* level )
{
	assert( !m_level );
	m_level = level;

	IDirect3DDevice9* device = m_level->GetGame()->GetRenderer()->GetDevice().m_d3d9;

	D3DVIEWPORT9 viewport;
	device->GetViewport( &viewport );

	m_width = viewport.Width;
	m_height = viewport.Height;
	m_aspectRatio = static_cast< float >( m_width ) / static_cast< float >( m_height );
}

void CameraManager::ShutDown()
{
	assert( m_level );
	m_level = nullptr;
}

void CameraManager::Render() const
{
	IDirect3DDevice9* device = m_level->GetGame()->GetRenderer()->GetDevice().m_d3d9;

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up.
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPosition = m_eyePosition + m_lookDirection * 50.0f;
	D3DXVECTOR3 upVector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &m_eyePosition, &lookAtPosition, &upVector );
	device->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perspective transform, we need the field of view, the aspect ratio,
	// and the near and far clipping planes.
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, m_fov, m_aspectRatio, m_near, m_far );
	device->SetTransform( D3DTS_PROJECTION, &matProj );
}
