#include "PhysicsEngine.h"

#include "SphereCollider.h"
#include "OrientedBoxCollider.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;



PhysicsEngine::PhysicsEngine(long double timeStepSeconds)
{
	m_timeStep = timeStepSeconds;
	m_deltaTime = 0;
}
PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::updateScene(Ref<Scene> activeScene)
{
	m_scene = activeScene;
}

vector<Ref<Collider>> PhysicsEngine::checkCollide(const Ref<Collider>& collider) //notera att denna funktion inte tar hänsyn till om objekten redan har kolliderat
{
	// kollar bara en collider per gameobject
	vector<Ref<Collider>> colliderVec;
	for (auto& go : m_scene->getSceneGameObjects())
	{
		Ref<Collider> other = go->getComponentType<Collider>(Component::ComponentEnum::COLLIDER);
		if (other != nullptr)
		{
			if (collider->collide(other))
			{
				Ref<Logic> logic = go->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
				if (logic != nullptr)
				{
					logic->onCollision(collider);
				}
				logic = collider->getGameObject()->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
				if (logic != nullptr)
				{
					logic->onCollision(other);
				}
				colliderVec.push_back(other);
			}
		}
	}
	return colliderVec;
}

void PhysicsEngine::simulate(long double dt)
{
	for (auto& go : m_scene->getSceneGameObjects())
	{
		Ref<RigidBody> rg = go->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
		if (rg != nullptr)
		{
			internalSimulate(rg, dt);
		}
	}
}

void PhysicsEngine::internalSimulate(const Ref<RigidBody>& rigidBody, long double dt)
{
	m_deltaTime += dt;
	while (m_timeStep < m_deltaTime)
	{
		
		rigidBody->getTransform()->setPosition(calcPos(rigidBody));

		Ref<Collider> rigidBodyCollider = rigidBody->getGameObject()->getComponentType<SphereCollider>(Component::ComponentEnum::COLLIDER);
		if (rigidBodyCollider != nullptr)
		{
			vector<Ref<Collider>> otherColliders = checkCollide(rigidBodyCollider);

			//sphere vs obb collision
			if ((rigidBodyCollider->getType() & Component::ComponentEnum::SPHERE_COLLIDER) == Component::ComponentEnum::SPHERE_COLLIDER)
			{
				for (auto& other : otherColliders)
				{
					if ((other->getType() & Component::ComponentEnum::ORIENTED_BOX_COLLIDER) == Component::ComponentEnum::ORIENTED_BOX_COLLIDER)
					{

						Vector3 normal = sphereCollideObb(rigidBodyCollider, other);
						//DirectX::
						rigidBody->stop();
					}


				}
			}
		}
		

		m_deltaTime -= m_timeStep;
	}
}


 
Vector3 PhysicsEngine::calcPos(const Ref<RigidBody>& rigidBody)
{
	rigidBody->m_acceleration = rigidBody->m_force / rigidBody->m_mass;
	rigidBody->m_acceleration.y -= rigidBody->m_gravity;

	rigidBody->m_velocity += rigidBody->m_acceleration * (float)m_timeStep;

	return rigidBody->m_transform->getPosition() + rigidBody->m_velocity * (float)m_timeStep;
}


Vector3 PhysicsEngine::sphereCollideObb(const Ref<Collider>& sphere, const Ref<Collider>& obb)
{
	assert(obb->getGameObject()->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY) == nullptr); //sånt hanteras inte


	Vector3 normal = Vector3(0, 1, 0);
	Vector3 position = std::dynamic_pointer_cast<OrientedBoxCollider>(obb)->closestPointOnObb(
		std::dynamic_pointer_cast<SphereCollider>(sphere)->getInternalCollider().Center, normal);



	sphere->getTransform()->setPosition((position + normal * 1.001f * std::dynamic_pointer_cast<SphereCollider>(sphere)->getInternalCollider().Radius));
	return normal;
}