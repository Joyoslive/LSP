#pragma once

#include "GameObject.h"

#include <vector>

class PhysicsEngine
{
private:
	


public:
	PhysicsEngine();
	~PhysicsEngine();

	Ref<Collider> Collide(Ref<Collider> collider);

	

	std::vector<Ref<GameObject>> m_tempScene;
};

