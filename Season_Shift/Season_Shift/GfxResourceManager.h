#pragma once
#include "DXResourceManager.h"

/*

Works as a manager for high level graphics resources. Holds repositories for currently loaded graphics resources.

Creates (Assembles them with DXResources), Destroys and Manages
- Models
- Meshes
- Material

*/
class GfxResourceManager
{
private:
	std::shared_ptr<DXResourceManager> m_dxResourceManager;

public:
	GfxResourceManager(std::shared_ptr<DXResourceManager> resourceManager);
	~GfxResourceManager();

};

