#include "TextureManager.h"
#include "Texture.h"
#include <d3dx9tex.h>
#include <algorithm>
#include <cassert>

TextureManager::TextureManager()
	: m_device( nullptr )
{
}

TextureManager::~TextureManager()
{
	assert( !m_device );
}

void TextureManager::StartUp( IDirect3DDevice9* device )
{
	assert( device );
	assert( !m_device );
	assert( m_textures.empty() );
	m_device = device;
}

void TextureManager::ShutDown()
{
	assert( m_device );
	m_device = nullptr;
	assert( m_textures.empty() );
}

Texture TextureManager::LoadTexture( const std::string& textureFile )
{
	auto foundIt = std::find_if( m_textures.begin(), m_textures.end(), [&textureFile]( std::pair< std::string, Texture >& namedTexture ) { return namedTexture.first == textureFile; } );

	if( foundIt != m_textures.end() )
	{
		foundIt->second.texture->AddRef();
		return foundIt->second;
	}

	Texture newTexture = {};
	D3DXCreateTextureFromFileA( m_device, textureFile.c_str(), &newTexture.texture );

	if( newTexture )
	{
		m_textures.emplace_back( textureFile, newTexture );
		return newTexture;
	}

	assert( false );
	return Texture{};
}

void TextureManager::UnloadTexture( const Texture& texture )
{
	auto foundIt = std::find_if( m_textures.begin(), m_textures.end(), [&texture]( std::pair< std::string, Texture >& namedTexture ) { return namedTexture.second.texture == texture.texture; } );
	assert( foundIt != m_textures.end() );

	if( foundIt != m_textures.end() )
	{
		if( !foundIt->second.texture->Release() )
		{
			if( foundIt != m_textures.end() - 1 )
			{
				std::iter_swap( foundIt, m_textures.end() - 1 );
			}
			m_textures.pop_back();
		}
	}
}
