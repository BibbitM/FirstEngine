#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Texture.h"
#include <string>
#include <vector>

class ShapeAabb;
struct D3DXCOLOR;
struct D3DXVECTOR3;

class Terrain : public Object
{
public:
	Terrain();
	~Terrain();

	void SetTerrainFile( const std::string& terrainFile );

	const std::string& GetTerrainFile() const { return m_terrainFile; }

	float GetHeight( float x, float y ) const;
	float GetHeight( const D3DXVECTOR3& pos ) const;

	int GetCount() const { return m_count; }

	ShapeAabb GetCollisionShape( int x, int z ) const;

protected:
	virtual void OnStartUp() override;
	virtual void OnShutDown() override;
	virtual void OnRender( FrameRenderer& frame ) const override;

private:
	void LoadFromFile();
	void LoadResources();
	void UnloadResources();

	float GetHeight( int index ) const;

	int GetIndex( float x, float z ) const;
	int GetIndex( int x, int z ) const;

	D3DXVECTOR3 GetCorner() const;
	D3DXVECTOR3 GetPosition( int x, int z ) const;

	static D3DXCOLOR GetColorForHeight( float height );

	std::string m_terrainFile;

	float m_size;
	float m_height;
	int m_count;

	std::vector< int > m_tiles;

	Mesh m_boxMesh;
	Texture m_grassTexture;
	Texture m_lavaTexture;

	static const std::string s_boxMeshName;
	static const std::string s_grassTextureName;
	static const std::string s_lavaTextureName;

};
