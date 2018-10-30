#pragma once

#include "PlatformDefinitions.h"

struct IDirect3DTexture9;

struct Texture
{
	IDirect3DTexture9* texture = nullptr;

	operator bool() const { return !!texture; }
	void clear()
	{
		*this = Texture{};
	}
};
