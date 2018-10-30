#pragma once

#include "PlatformDefinitions.h"

struct ID3DXMesh;

struct Mesh
{
	ID3DXMesh* mesh = nullptr;
	DWORD numMaterials = 0;

	operator bool() const { return !!mesh; }
	void clear()
	{
		*this = Mesh{};
	}
};
