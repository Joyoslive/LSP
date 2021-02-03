#pragma once

#include "GameObject.h"
#include "Scene.h"

#include <vector>

class PhysicsEngine
{
private:
	Ref<Scene> m_scene;


public:
	PhysicsEngine();
	~PhysicsEngine();

	void addScene(Ref<Scene> scene);
	std::vector<Ref<Collider>> checkCollide(Ref<Collider> collider);
};

