#pragma once

#include "NonCopyable.h"
#include <memory>
#include "Timer.h"

class IRenderer;
class FrameRenderer;
class InputManager;
class Level;
class MeshManager;
class Renderer;
class TextureManager;
struct RendererInitContext;

class Game : private NonCopyable
{
public:
	Game();
	~Game();

	/** Start up the game and all subsystems.
		@return true if succeeded */
	bool StartUp( const RendererInitContext& rendererContext );
	/** Shut down all subsystems and the game */
	void ShutDown();

	/** Main game loop.
		Must be called periodically. */
	void Loop();

	IRenderer* GetRenderer() const;

	MeshManager* GetMeshManager() const;

	TextureManager* GetTextureManager() const;

	InputManager* GetInputManager() const;

	float GetTime() const { return m_timer.GetTimeSeconds(); }
	float GetDeltaTime() const { return m_timer.GetDeltaTimeSeconds(); }

private:
	void Update();
	void Render();

	std::unique_ptr< Renderer > m_renderer;
	std::unique_ptr< Level > m_level;
	std::unique_ptr< FrameRenderer > m_frameRenderer;
	std::unique_ptr< MeshManager > m_meshManager;
	std::unique_ptr< TextureManager > m_textureManager;
	std::unique_ptr< InputManager > m_inputManager;
	Timer m_timer;
};
