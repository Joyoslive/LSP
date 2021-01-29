#pragma once
#include "GfxCore.h"
#include <vector>

class IRenderStrategy
{
private:

public:
	IRenderStrategy();
	virtual ~IRenderStrategy();
	virtual void render() = 0;
};

