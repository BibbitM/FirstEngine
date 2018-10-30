#include "Game.h"
#include "Level.h"
#include "CameraManager.h"
#include "FrameRenderer.h"
#include "InputManager.h"
#include "MeshManager.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "RendererDevice.h"
#include <d3dx9math.h>
#include <cassert>

Game::Game()
	: m_renderer( std::make_unique< Renderer >() )
	, m_level( std::make_unique< Level >() )
	, m_frameRenderer( std::make_unique< FrameRenderer >() )
	, m_meshManager( std::make_unique< MeshManager >() )
	, m_textureManager( std::make_unique< TextureManager >() )
	, m_inputManager( std::make_unique< InputManager >() )
{
	// Do nothing here, @see StartUp.
}

Game::~Game()
{
	// Do nothing here, @see ShutDown.
}

bool Game::StartUp(const RendererInitContext& rendererContext)
{
	if (!m_renderer->StartUp(rendererContext))
		return false;

	m_meshManager->StartUp( m_renderer->GetDevice().m_d3d9 );

	m_textureManager->StartUp( m_renderer->GetDevice().m_d3d9 );

	m_frameRenderer->StartUp();

	m_inputManager->StartUp();

	m_level->StartUp( this );

	return true;
}

void Game::ShutDown()
{
	m_level->ShutDown();

	m_inputManager->ShutDown();

	m_frameRenderer->ShutDown();

	m_textureManager->ShutDown();

	m_meshManager->ShutDown();

	m_renderer->ShutDown();
}

void Game::Loop()
{
	Update();

	Render();
}

IRenderer* Game::GetRenderer() const
{
	assert(m_renderer);
	return m_renderer.get();
}

MeshManager* Game::GetMeshManager() const
{
	assert( m_meshManager );
	return m_meshManager.get();
}

TextureManager* Game::GetTextureManager() const
{
	assert( m_textureManager );
	return m_textureManager.get();
}

InputManager* Game::GetInputManager() const
{
	assert( m_inputManager );
	return m_inputManager.get();
}

void Game::Update()
{
	m_timer.Update();

	m_inputManager->Update();

	m_level->Update(GetDeltaTime());
}

void Game::Render()
{
	m_frameRenderer->Reset();

	m_level->Render( *m_frameRenderer );

	//#DEBUG
	{
		m_frameRenderer->AddText( 0, 0, 0xFFFFFFFF, "FPS: %.4f", 1.0f / m_timer.GetDeltaTimeSeconds() );
	}

	if (m_renderer->BeginRender())
	{
		m_renderer->Render( *m_frameRenderer );

		m_renderer->EndRender();
	}

	m_renderer->Present();
}
