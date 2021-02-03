#pragma once

#include "GameObject.h"
#include "Scene.h"

#include <vector>

class PhysicsEngine
{
private:
	//Ref<Scene> = 


public:
	PhysicsEngine();
	~PhysicsEngine();

	void addScene(Ref<Scene> scene);
	std::vector<Ref<GameObject>> m_tempScene;
};

