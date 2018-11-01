#pragma once

#include "NonCopyable.h"

class FrameRenderer;
class Level;

class Object : public NonCopyable
{
public:
	Object();
	virtual ~Object();

	void StartUp( Level* level );
	void ShutDown();
	void Destroy();

	virtual void Update( float deltaTime );
	virtual void Render( FrameRenderer& frame ) const;

	Level* GetLevel() const;

protected:
	virtual void OnStartUp();
	virtual void OnShutDown();

private:
	Level* m_level;
};
