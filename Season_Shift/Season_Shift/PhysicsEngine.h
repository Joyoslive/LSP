#pragma once

#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"
#include <vector>

class PhysicsEngine
{
private:
	Ref<Scene> m_scene;

	void calcPos(Ref<RigidBody>& rigidBody);
	Timer m_timer;
	long double m_timeStep;
	long double m_deltaTime;

public:
	PhysicsEngine(long double timeStepSeconds = 1.0 / 120.0);
	~PhysicsEngine();

	void addScene(Ref<Scene> scene);
	std::vector<Ref<Collider>> checkCollide(Ref<Collider> collider);
	
	void simulate(Ref<RigidBody> rigidBody);
};

