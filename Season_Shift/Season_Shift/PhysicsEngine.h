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

	
	
	long double m_timeStep;
	long double m_deltaTime;
	

	void internalSimulate(const Ref<RigidBody>& rigidBody, long double dt);


private:
	DirectX::SimpleMath::Vector3 calcPos(const Ref<RigidBody>& rigidBody);
	std::vector<Ref<Collider>> checkCollide(const Ref<Collider>& collider);
	DirectX::SimpleMath::Vector3 sphereCollideObb(const Ref<Collider>& sphere, const Ref<Collider>& obb);

public:
	PhysicsEngine(long double timeStepSeconds = 1.0 / 120.0);
	~PhysicsEngine();

	void updateScene(Ref<Scene> activeScene) override;
	void simulate(long double dt);
};

