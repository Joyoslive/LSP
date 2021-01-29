#pragma once
#include "DXDevice.h"
#include "DXResourceManager.h"
#include "GfxResourceManager.h"

/*

Works as a creator for high level graphics resources. Holds repositories for currently loaded graphics resources.

Adds (Assembles them with DXResources), removes, manages existing
- Models
- Meshes
- Material
- ShaderProgram

*/
class GfxResourceDevice
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

	std::unique_ptr<GfxResourceManager> m_resourceManager;

public:
	GfxResourceDevice(std::shared_ptr<DXDevice> dev);
	~GfxResourceDevice();

	/*

	Create Mesh
	Create Model
	Create Material
	Create ShaderProgram
	Create ShadingPackage --> ShaderProgram + Material? (Simplify for this project development)
	
	*/
};

