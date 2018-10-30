#pragma once

#include <d3dx9.h>

#define FVF_VERTEXXYZUV ( D3DFVF_XYZ | D3DFVF_TEX1 )

struct VertexXyzUv
{
	FLOAT x, y, z; // Position for the vertex.
	FLOAT u, v;    // The texture mapping.
};

