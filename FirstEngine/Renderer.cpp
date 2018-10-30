#include "Renderer.h"
#include "RendererInitContext.h"
#include "RendererDevice.h"
#include "FrameRenderer.h"
#include "LineRenderer.h"
#include "MeshRenderer.h"
#include "TextRenderer.h"
#include <cassert>
#include <d3dx9.h>

Renderer::Renderer()
	: m_direct3d( nullptr )
	, m_direct3dDevice( nullptr )
	, m_meshRenderer( nullptr )
	, m_lineRenderer( nullptr )
	, m_textRenderer( nullptr )
{
}

Renderer::~Renderer()
{
	// Make sure that ShutDown was called.
	assert( !m_direct3d );
	assert( !m_direct3dDevice );
	assert( !m_meshRenderer );
	assert( !m_lineRenderer );
	assert( !m_textRenderer );
}

RendererDevice Renderer::GetDevice() const
{
	return RendererDevice{ m_direct3dDevice };
}

bool Renderer::StartUp(const RendererInitContext& rendererContext)
{
	// Create the D3D object.
	if( nullptr == ( m_direct3d = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}

	if( !CreateDevice( rendererContext ) )
	{
		return false;
	}

	SetupDevice();

	CreateRenderers();

	return true;
}

void Renderer::ShutDown()
{
	DestroyRenderers();

	if( m_direct3dDevice != nullptr )
		m_direct3dDevice->Release();
	m_direct3dDevice = nullptr;

	if (m_direct3d != nullptr)
		m_direct3d->Release();
	m_direct3d = nullptr;
}

bool Renderer::BeginRender()
{
	// Clear the backbuffer and the zbuffer
	m_direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(53, 53, 53), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(m_direct3dDevice->BeginScene()))
	{
		// Setup the world, view, and projection matrices

		// Set up world matrix
		//D3DXMATRIXA16 matWorld;
		//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
		//m_direct3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		return true;
	}

	return false;
}

void Renderer::Render( const FrameRenderer& frame )
{
	m_meshRenderer->Render( frame.GetMeshes() );
	m_lineRenderer->Render( frame.GetLines() );
	m_textRenderer->Render( frame.GetTexts() );
}

void Renderer::EndRender()
{
	// End the scene
	m_direct3dDevice->EndScene();
}

void Renderer::Present()
{
	// Present the backbuffer contents to the display
	m_direct3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

bool Renderer::CreateDevice( const RendererInitContext& rendererContext )
{
	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if( FAILED( m_direct3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, rendererContext.hWnd,
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING /*D3DCREATE_HARDWARE_VERTEXPROCESSING*/,
										  &d3dpp, &m_direct3dDevice ) ) )
	{
		return false;
	}

	return true;
}

void Renderer::SetupDevice()
{
	// Turn on the zbuffer
	m_direct3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	// Turn off lighting
	//m_direct3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on ambient lighting
	m_direct3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
}

void Renderer::CreateRenderers()
{
	assert( !m_meshRenderer );
	assert( !m_lineRenderer );
	assert( !m_textRenderer );

	m_meshRenderer = new MeshRenderer();
	m_meshRenderer->StartUp( m_direct3dDevice );

	m_lineRenderer = new LineRenderer();
	m_lineRenderer->StartUp( m_direct3dDevice );

	m_textRenderer = new TextRenderer();
	m_textRenderer->StartUp( m_direct3dDevice );
}

void Renderer::DestroyRenderers()
{
	assert( m_meshRenderer );
	assert( m_lineRenderer );
	assert( m_textRenderer );

	if( m_textRenderer )
	{
		m_textRenderer->ShutDown();
		delete m_textRenderer;
		m_textRenderer = nullptr;
	}

	if( m_lineRenderer )
	{
		m_lineRenderer->ShutDown();
		delete m_lineRenderer;
		m_lineRenderer = nullptr;
	}

	if( m_meshRenderer )
	{
		m_meshRenderer->ShutDown();
		delete m_meshRenderer;
		m_meshRenderer = nullptr;
	}
}
