#include "ForwardRenderStrategy.h"

ForwardRenderStrategy::ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
{
	OutputDebugStringW(L"Using forward render.\n");

	/*
	
	Create the neccessary pipeline state, buffers and textures to allow for basic flat-shaded rendering of different meshes.

	MeshCBuffer
		wm
		vm
		pj

	PointSampler

	VS, PS

	InputLayout

	DSV

	Default Texture

	*/
}

ForwardRenderStrategy::~ForwardRenderStrategy()
{
}

void ForwardRenderStrategy::render()
{
	auto dxDev = m_renderer->getDXDevice();

	dxDev->clearScreen();

	// Do lots of rendering stuff with the help of Renderer and various Binder/Technique abstractions in mind! etc.
	
	/*

	m_renderer->setPipelineState(defaultForward);
	
	for (auto& go : gameObjects)
	{
		auto& mesh = go.mesh;			--> vb, ib
		auto& material = go.material;	--> ps textures + shader

		m_renderer->drawMesh(mesh, material);
		
	}
	
	*/



	dxDev->present();
}
