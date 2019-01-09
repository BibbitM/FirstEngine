#include "PhysicsCharacter.h"

class GenericCamera;

class PacMan : public PhysicsCharacter
{
public:
	PacMan();
	~PacMan();

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

	virtual void OnUpdate( float deltaTime ) override;

private:
	void UpdateInput();

	GenericCamera* m_camera;
};
