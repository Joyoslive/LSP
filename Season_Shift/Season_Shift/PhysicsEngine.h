#pragma once

#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"
#include "Collider.h"
#include "RigidBody.h"
#include "SceneManagerObserver.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>

class PhysicsEngine : public SceneManagerObserver
{
public:
	PhysicsEngine(long double timeStepSeconds = 1.0 / 120.0);
	~PhysicsEngine();



private:
	Ref<Scene> m_scene;

	
	
	long double m_timeStep;
	long double m_deltaTime;
	

	void internalSimulate(const Ref<RigidBody>& rigidBody, long double dt);


private:
	DirectX::SimpleMath::Vector3 calcPos(const Ref<RigidBody>& rigidBody);
	std::vector<Ref<Collider>> checkCollide(const Ref<Collider>& collider);

	void collisionResponse(const Ref<RigidBody>& rigidBody, const Ref<Collider>& rigidBodyCollider, const Ref<Collider>& obb);
	DirectX::SimpleMath::Vector3 sphereCollideObb(const Ref<Collider>& sphere, const Ref<Collider>& obb);
	DirectX::SimpleMath::Vector3 capsuleCollideObb(const Ref<Collider>& capsule, const Ref<Collider>& obb);

public:
	void updateScene(Ref<Scene> activeScene) override;
	void simulate(long double dt);
};

