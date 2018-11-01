#pragma once

#include "NonCopyable.h"

class FrameRenderer;
class Level;

class Object : public NonCopyable
{
public:
	Object();
	virtual ~Object();

	bool IsInitialized() const { return m_level != nullptr; }

	void StartUp( Level* level );
	void ShutDown();
	void Destroy();

	void Update( float deltaTime );
	void Render( FrameRenderer& frame ) const;

	Level* GetLevel() const;

protected:
	virtual void OnStartUp();
	virtual void OnShutDown();

	virtual void OnUpdate( float deltaTime );
	virtual void OnRender( FrameRenderer& frame ) const;

private:
	Level* m_level;
};
