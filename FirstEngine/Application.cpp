#include "Application.h"
#include "Game.h"
#include "InputManager.h"
#include "RendererInitContext.h"
#include "WindowInitContext.h"
#include <cassert>

namespace helper
{
	class GameInputListenerHook : public IWindowInputListener
	{
	public:
		GameInputListenerHook( InputManager* inputManger )
			: m_inputManger( inputManger )
		{
			assert( m_inputManger );
		}

		virtual void OnKeyPressed( int keyCode ) override
		{
			m_inputManger->OnKeyPressed( keyCode );
		}
		virtual void OnKeyReleased( int keyCode ) override
		{
			m_inputManger->OnKeyReleased( keyCode );
		}
		virtual void OnMouseMove( int mouseX, int mouseY ) override
		{
			m_inputManger->OnMouseMove( mouseX, mouseY );
		}

		void ReleaseAll()
		{
			for( int key = 0; key < InputManager::s_numKeys; ++key )
			{
				OnKeyReleased( key );
			}

			m_inputManger->OnMouseLost();
		}

	private:
		InputManager* m_inputManger;
	};
}

Application::Application( HINSTANCE hInstance, int cmdShow )
	: m_window( hInstance, cmdShow )
{
}

Application::~Application()
{
}

int Application::Run( int widht, int height )
{
	Game game;

	m_window.SetActiveAppListener( this );
	m_window.Create( WindowInitContext{widht, height} );

	int retCode = -1;

	if( game.StartUp( RendererInitContext{m_window.GetHWnd()} ) )
	{
		helper::GameInputListenerHook inputHook( game.GetInputManager() );
		bool wasActive = true;
		m_window.SetInputListener( &inputHook );

		m_window.Show();

		MSG msg = {};
		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				game.Loop();

				// Window is inactive so yield CPU time to other processes.
				if( !m_isActive )
				{
					if (wasActive)
					{
						// Release all presed keys.
						inputHook.ReleaseAll();
					}

					Sleep( 50 );
				}
				wasActive = m_isActive;
			}
		}

		retCode = (int)msg.wParam;

		m_window.SetInputListener( nullptr );
	}

	game.ShutDown();

	m_window.SetActiveAppListener( nullptr );

	return retCode;
}

bool Application::IsAppActive() const
{
	return m_isActive;
}

void Application::SetAppActive( bool activate )
{
	m_isActive = activate;
}
