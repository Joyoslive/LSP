#pragma once

#include "GameObject.h"

#include <vector>

class PhysicsEngine
{
private:
	


public:
	PhysicsEngine();
	~PhysicsEngine();

	
	std::vector<Ref<GameObject>> m_tempScene;
};

