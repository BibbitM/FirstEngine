#pragma once

struct IDirect3DDevice9;

struct RendererDevice 
{
	IDirect3DDevice9* m_d3d9 = nullptr;
};
