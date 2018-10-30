#pragma once

#include "NonCopyable.h"
#include "Window.h"
#include "PlatformDefinitions.h"
#include "WindowListeners.h"


class Application : private NonCopyable, public IWindowAcitveAppListener
{
public:
	Application(HINSTANCE hInstance, int cmdShow);

	~Application();

	/** Initialize game and run message loop. */
	int Run( int widht, int height );

	virtual bool IsAppActive() const override;
	virtual void SetAppActive( bool activate ) override;

private:
	Window m_window;
	bool m_isActive;
};

