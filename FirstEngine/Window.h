#pragma once

#include "NonCopyable.h"
#include "PlatformDefinitions.h"

struct WindowInitContext;

class IWindowAcitveAppListener;
class IWindowInputListener;

class Window : private NonCopyable
{
public:
	Window( HINSTANCE hInstance, int cmdShow );
	~Window();

	HWND GetHWnd() const { return m_hWnd; }

	void Create( const WindowInitContext& initContext );
	void Show();

	void SetActiveAppListener( IWindowAcitveAppListener* lisener );
	void SetInputListener( IWindowInputListener* lisener );

private:
	void DeclareHighDpiAware();
	void RegisterClass();
	void UnregisterClass();

	static LRESULT CALLBACK StaticWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT CALLBACK WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

	static const wchar_t s_windowClassName[];
	static const wchar_t s_windowName[];

	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_cmdShow;

	IWindowAcitveAppListener* m_activeAppListener;
	IWindowInputListener* m_inputListener;
};
