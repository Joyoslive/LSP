#pragma once
#include "DXDevice.h"

/*

Works as a manager for low-level graphics resources. Holds repositories for currently loaded graphics resources.

Creates, Destroys and Manages
- DXBuffers
- DXTextures
- Samplers
- RasterizerStates
- PipelineObjects

*/
class DXResourceManager
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

public:
	DXResourceManager(std::shared_ptr<DXDevice> dev);
	~DXResourceManager();

};

