#pragma once

#include "Actor.h"
#include "Mesh.h"
#include "Texture.h"
#include <string>

class Pawn : public Actor
{
public:
	Pawn( const std::string& meshName = std::string(),
		  const std::string& textureName = std::string(),
		  const D3DXCOLOR& color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		  const D3DXVECTOR3& position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
		  const D3DXVECTOR3& rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
		  const D3DXVECTOR3& scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	~Pawn();

	const std::string& GetMeshName() const { return m_meshName; }
	const std::string& GetTextureName() const { return m_textureName; }

	void SetMesh( const std::string& meshName );
	void SetTexture( const std::string& textureName );


	void SetColor( const D3DXCOLOR& color ) { m_color = color; }

	const D3DXCOLOR& GetColor() const { return m_color; }


	const D3DXVECTOR3& GetMeshPosition() const { return m_meshPosition; }
	const D3DXVECTOR3& GetMeshRotation() const { return m_meshRotation; }
	const D3DXVECTOR3& GetMeshScale() const { return m_meshScale; }

	D3DXMATRIX GetMeshLocalMatrix() const;
	D3DXMATRIX GetMeshWorldMatrix() const;

	void SetMeshPosition( const D3DXVECTOR3& position ) { m_meshPosition = position; }
	void SetMeshRotation( const D3DXVECTOR3& rotation ) { m_meshRotation = rotation; }
	void SetMeshScale( const D3DXVECTOR3& scale ) { m_meshScale = scale; }

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;

	virtual void OnRender( FrameRenderer& frame ) const override;

private:
	void LoadMesh();
	void LoadTexture();

	void UnloadMesh();
	void UnloadTexture();

	std::string m_meshName;
	std::string m_textureName;

	Mesh m_mesh;
	Texture m_texture;
	D3DXCOLOR m_color;

	D3DXVECTOR3 m_meshPosition;
	D3DXVECTOR3 m_meshRotation;
	D3DXVECTOR3 m_meshScale;
};