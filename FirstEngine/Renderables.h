#pragma once

#include "PlatformDefinitions.h"
#include "Mesh.h"
#include "Texture.h"
#include <d3d9types.h>
#include <d3dx9math.h>
#include <string>

struct RenderableMesh
{
	D3DXMATRIX matrix;
	Mesh mesh;
	Texture texture;
	D3DCOLOR color;
};

struct RenderableLine
{
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;
	float width;
	D3DCOLOR color;
};

struct RenderableText
{
	int x;
	int y;
	std::string text;
	D3DCOLOR color;
};