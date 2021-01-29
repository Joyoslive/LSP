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

*/
class GfxResourceDevice
{
private:


public:
	GfxResourceDevice();
	~GfxResourceDevice();
};

