#include "InputManager.h"

InputManager::InputManager()
	: m_lockCursor( false )
	, m_showCursor( true )
	, m_mousePositionX( 0 )
	, m_mousePositionY( 0 )
	, m_mouseMoveX( 0 )
	, m_mouseMoveY( 0 )
	, m_mouseMoveXAccumulator( 0 )
	, m_mouseMoveYAccumulator( 0 )
	, m_mouseWheel( 0 )
	, m_mouseWheelAccumulator( 0 )
{
	memset( m_keyStates, 0, sizeof( m_keyStates ) );
}

InputManager::~InputManager()
{
}

void InputManager::StartUp()
{
}

void InputManager::ShutDown()
{
}

void InputManager::Update()
{
	UpdateKeys();

	UpdateMouse();
}

void InputManager::UpdateKeys()
{
	for( int key = 0; key < s_numKeys; ++key )
	{
		if( ( m_keyStates[ key ] & PRESSED ) != 0 )
		{
			if( ( m_keyStates[ key ] & WAS_PRESSED ) == 0 )
			{
				m_keyStates[ key ] |= JUST_PRESSED;
			}
			else
			{
				m_keyStates[ key ] &= ~JUST_PRESSED;
			}
			m_keyStates[ key ] |= WAS_PRESSED;
			m_keyStates[ key ] &= ~JUST_RELEASED;
		}
		else
		{
			if( ( m_keyStates[ key ] & WAS_PRESSED ) != 0 )
			{
				m_keyStates[ key ] |= JUST_RELEASED;
			}
			else
			{
				m_keyStates[ key ] &= ~JUST_RELEASED;
			}
			m_keyStates[ key ] &= ~JUST_PRESSED;
			m_keyStates[ key ] &= ~WAS_PRESSED;
		}
	}
}

void InputManager::UpdateMouse()
{
	m_mouseMoveX = m_mouseMoveXAccumulator;
	m_mouseMoveY = m_mouseMoveYAccumulator;
	m_mouseMoveXAccumulator = 0;
	m_mouseMoveYAccumulator = 0;


	m_mouseWheel = m_mouseWheelAccumulator;
	m_mouseWheelAccumulator = 0;
}

byte InputManager::GetKeyState( int keyCode ) const
{
	if( !IsValidKey( keyCode ) )
	{
		return 0;
	}

	return m_keyStates[ keyCode ];
}

bool InputManager::IsKeyPressed( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & PRESSED ) != 0;
}

bool InputManager::IsKeyJustPressed( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & JUST_PRESSED ) != 0;
}

bool InputManager::IsKeyReleased( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & PRESSED ) == 0;
}

bool InputManager::IsKeyJustReleased( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & JUST_RELEASED ) != 0;
}

int InputManager::GetMousePositionX() const
{
	return m_mousePositionX;
}

int InputManager::GetMousePositionY() const
{
	return m_mousePositionY;
}

int InputManager::GetMouseMoveX() const
{
	return m_mouseMoveX;
}

int InputManager::GetMouseMoveY() const
{
	return m_mouseMoveY;
}

int InputManager::GetMouseWheel() const
{
	return m_mouseWheel;
}

void InputManager::OnKeyPressed( int keyCode )
{
	if( !IsValidKey( keyCode ) )
	{
		return;
	}

	m_keyStates[ keyCode ] |= PRESSED;
}

void InputManager::OnKeyReleased( int keyCode )
{
	m_keyStates[ keyCode ] &= ~PRESSED;
}

void InputManager::OnMouseMove( int mouseMoveX, int mouseMoveY )
{
	m_mouseMoveXAccumulator += mouseMoveX;
	m_mouseMoveYAccumulator += mouseMoveY;
}

void InputManager::OnMousePosition( int mousePositionX, int mousePositionY )
{
	m_mousePositionX = mousePositionX;
	m_mousePositionY = mousePositionY;
}

void InputManager::OnMouseWheel( int mouseWheelDelta )
{
	m_mouseWheelAccumulator += mouseWheelDelta;
}

void InputManager::OnMouseLost()
{
}
