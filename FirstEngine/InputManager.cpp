#include "InputManager.h"

InputManager::InputManager()
	: m_wasMouseMove( false )
	, m_mousePosX( 0 )
	, m_mousePosY( 0 )
	, m_prevMousePosX( 0 )
	, m_prevMousePosY( 0 )
	, m_mouseMoveX( 0 )
	, m_mouseMoveY( 0 )
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
		if( ( m_keyStates[ key ] & PRESSED_CURR ) != 0 )
		{
			m_keyStates[ key ] |= PRESSED_PREV;
		}
		else
		{
			m_keyStates[ key ] &= ~PRESSED_PREV;
		}
	}
}

void InputManager::UpdateMouse()
{
	if( m_wasMouseMove )
	{
		m_mouseMoveX = m_mousePosX - m_prevMousePosX;
		m_mouseMoveY = m_mousePosY - m_prevMousePosY;
	}
	else
	{
		m_mouseMoveX = 0;
		m_mouseMoveY = 0;
	}

	m_prevMousePosX = m_mousePosX;
	m_prevMousePosY = m_mousePosY;
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
	return ( keyState & PRESSED_CURR ) != 0;
}

bool InputManager::WasKeyJustPressed( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & PRESSED_CURR ) != 0 && ( keyState & PRESSED_PREV ) == 0;
}

bool InputManager::IsKeyReleased( int keyCode ) const
{
	const byte keyState = GetKeyState( keyCode );
	return ( keyState & PRESSED_CURR ) == 0;
}

int InputManager::GetMousePositionX() const
{
	return m_mousePosX;
}

int InputManager::GetMousePositionY() const
{
	return m_mousePosY;
}

int InputManager::GetMouseMoveX() const
{
	return m_mouseMoveX;
}

int InputManager::GetMouseMoveY() const
{
	return m_mouseMoveY;
}

void InputManager::OnKeyPressed( int keyCode )
{
	if( !IsValidKey( keyCode ) )
	{
		return;
	}

	m_keyStates[ keyCode ] |= PRESSED_CURR;
}

void InputManager::OnKeyReleased( int keyCode )
{
	m_keyStates[ keyCode ] &= ~PRESSED_CURR;
}

void InputManager::OnMouseMove( int mouseX, int mouseY )
{
	m_mousePosX = mouseX;
	m_mousePosY = mouseY;

	m_wasMouseMove = true;
}

void InputManager::OnMouseLost()
{
	m_wasMouseMove = false;
}
