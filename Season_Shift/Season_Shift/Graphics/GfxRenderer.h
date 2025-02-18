#pragma once
#include "DX/DXDevice.h"

/*

Works as a graphics "Context" helper
- Draws
- SetPipelines
- SetCamera
- Combination of graphics context usage for low-level wrappers and slightly higher-level wrappers (such as Mesh/Material or slightly lower-level like Pipeline) in order to simplify development

Used in RenderStrategy implementations where the main graphic technique implementations
*/ 
class GfxRenderer
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

public:
	/*

	
	*/

	GfxRenderer(std::shared_ptr<DXDevice> dev);
	~GfxRenderer();

	/*
	
	setCamera
	setPipeline(DXPipelineState)
	drawMesh(mesh, material);
	drawMeshInstanced(mesh, material, instanceData)

	*/


	/*
	Expose DXDevice to allow for low-level graphic resource creation for techniques for RenderStrategy
	Raw pointer represents non-owning relationship
	*/
	DXDevice* const getDXDevice();

};

