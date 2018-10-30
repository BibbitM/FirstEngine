#pragma once

class IWindowAcitveAppListener
{
public:
	virtual bool IsAppActive() const = 0;
	virtual void SetAppActive( bool activate ) = 0;

protected:
	virtual ~IWindowAcitveAppListener(){}
};


class IWindowInputListener
{
public:
	void OnKey( int keyCode, bool pressed )
	{
		if( pressed )
		{
			OnKeyPressed( keyCode );
		}
		else
		{
			OnKeyReleased( keyCode );
		}
	}

	virtual void OnKeyPressed( int keyCode ) = 0;
	virtual void OnKeyReleased( int keyCode ) = 0;

	virtual void OnMouseMove( int mouseX, int mouseY ) = 0;

protected:
	virtual ~IWindowInputListener() {}
};
