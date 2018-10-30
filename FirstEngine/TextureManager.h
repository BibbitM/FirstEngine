#pragma once

#include "NonCopyable.h"
#include <vector>

struct Texture;
struct IDirect3DDevice9;

class TextureManager : public NonCopyable
{
public:
	TextureManager();
	~TextureManager();

	void StartUp( IDirect3DDevice9* device );
	void ShutDown();

	Texture LoadTexture( const std::string& textureFile );
	void UnloadTexture( const Texture& texture );

private:
	IDirect3DDevice9* m_device;

	std::vector< std::pair< std::string, Texture > > m_textures;
};
