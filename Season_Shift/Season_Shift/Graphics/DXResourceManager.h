#pragma once

/*

Works as a manager for low-level graphics resources. Holds repositories for currently loaded graphics resources.

Adds, removes, manages existing
- DXBuffers
- DXTextures
- Samplers
- RasterizerStates
- PipelineObjects

*/
class DXResourceManager
{
private:


	/*
	
	some persistent storage
	
	*/

public:
	DXResourceManager();
	~DXResourceManager();

};

