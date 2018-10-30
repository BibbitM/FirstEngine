#pragma once

#include "IRenderer.h"
#include "PlatformDefinitions.h"

class FrameRenderer;
class LineRenderer;
class MeshRenderer;
class TextRenderer;

struct IDirect3D9;
struct IDirect3DDevice9;
struct RendererInitContext;
struct RendererDevice; //#TODO: Remove RendererDevice and IRenderer class.

class Renderer : public IRenderer
{
public:
	Renderer();
	~Renderer();

	virtual RendererDevice GetDevice() const override;

	bool StartUp( const RendererInitContext& rendererContext );

	void ShutDown();

	bool BeginRender();
	void Render( const FrameRenderer& frame );
	void EndRender();
	void Present();

private:
	bool CreateDevice( const RendererInitContext& rendererContext );
	void SetupDevice();

	void CreateRenderers();
	void DestroyRenderers();

	IDirect3D9* m_direct3d;
	IDirect3DDevice9* m_direct3dDevice;

	MeshRenderer* m_meshRenderer;
	LineRenderer* m_lineRenderer;
	TextRenderer* m_textRenderer;

};

