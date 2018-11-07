#pragma once

#include "PlatformDefinitions.h"
#include "NonCopyable.h"

class InputManager : private NonCopyable
{
public:
	static const int s_numKeys = 255;

	static const byte PRESSED = 1;
	static const byte WAS_PRESSED = 2;
	static const byte JUST_PRESSED = 4;
	static const byte JUST_RELEASED = 8;

	InputManager();
	~InputManager();

	void StartUp();
	void ShutDown();

	void Update();

	void SetLockCursor( bool lockCursor ) { m_lockCursor = lockCursor; }
	bool GetLockCursor() const { return m_lockCursor; }

	void SetShowCursor( bool showCursor ) { m_showCursor = showCursor; }
	bool GetShowCursor() const { return m_showCursor; }

	byte GetKeyState( int keyCode ) const;

	bool IsKeyPressed( int keyCode ) const;
	bool IsKeyJustPressed( int keyCode ) const;

	bool IsKeyReleased( int keyCode ) const;
	bool IsKeyJustReleased( int keyCode ) const;

	int GetMousePositionX() const;
	int GetMousePositionY() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;

	int GetMouseWheel() const;


	void OnKeyPressed( int keyCode );
	void OnKeyReleased( int keyCode );

	void OnMouseMove( int mouseMoveX, int mouseMoveY );
	void OnMousePosition( int mousePositionX, int mousePositionY );
	void OnMouseWheel( int mouseWheelDelta );

	void OnMouseLost();

private:
	void UpdateKeys();
	void UpdateMouse();

	static bool IsValidKey( int keyCode )
	{
		return keyCode >= 0 && keyCode < s_numKeys;
	}

	byte m_keyStates[ s_numKeys ];

	bool m_lockCursor;
	bool m_showCursor;

	int m_mousePositionX;
	int m_mousePositionY;

	int m_mouseMoveX;
	int m_mouseMoveY;
	int m_mouseMoveXAccumulator;
	int m_mouseMoveYAccumulator;

	int m_mouseWheel;
	int m_mouseWheelAccumulator;
};