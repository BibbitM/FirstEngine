#pragma once

#include "NonCopyable.h"

class FrameRenderer;
class Level;

class Object : public NonCopyable
{
public:
	Object();
	~Object();

	virtual void StartUp( Level* level );
	virtual void ShutDown();
	void Destroy();

	virtual void Update( float deltaTime );
	virtual void Render( FrameRenderer& frame ) const;

	Level* GetLevel() const;

private:
	Level* m_level;
};
