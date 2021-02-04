#pragma once

#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"
#include "SceneManagerObserver.h"
#include <vector>

class PhysicsEngine : public SceneManagerObserver
{
private:
	Ref<Scene> m_scene;

	DirectX::SimpleMath::Vector3 calcPos(Ref<RigidBody>& rigidBody);
	Timer m_timer;
	long double m_timeStep;
	long double m_deltaTime;
	std::vector<Ref<Collider>> checkCollide(Ref<Collider> collider);

	std::vector<Ref<Collider>> rigidBodyCollide(Ref<RigidBody>& rigidBody);

public:
	PhysicsEngine(long double timeStepSeconds = 1.0 / 120.0);
	~PhysicsEngine();

	void updateScene(Ref<Scene> activeScene) override;
	void simulate(Ref<RigidBody> rigidBody);
};

