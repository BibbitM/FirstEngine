#pragma once

#include "PlatformDefinitions.h"
#include "NonCopyable.h"

class InputManager : private NonCopyable
{
public:
	static const int s_numKeys = 255;

	static const byte PRESSED_CURR = 1;
	static const byte PRESSED_PREV = 2;

	InputManager();
	~InputManager();

	void StartUp();
	void ShutDown();

	void Update();

	byte GetKeyState( int keyCode ) const;

	bool IsKeyPressed( int keyCode ) const;
	bool WasKeyJustPressed( int keyCode ) const;

	bool IsKeyReleased( int keyCode ) const;

	int GetMousePositionX() const;
	int GetMousePositionY() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;


	void OnKeyPressed( int keyCode );
	void OnKeyReleased( int keyCode );

	void OnMouseMove( int mouseX, int mouseY );

	void OnMouseLost();

private:
	void UpdateKeys();
	void UpdateMouse();

	static bool IsValidKey( int keyCode )
	{
		return keyCode >= 0 && keyCode < s_numKeys;
	}

	byte m_keyStates[ s_numKeys ];

	bool m_wasMouseMove;

	int m_mousePosX;
	int m_mousePosY;

	int m_prevMousePosX;
	int m_prevMousePosY;

	int m_mouseMoveX;
	int m_mouseMoveY;
};