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

vector<Ref<Collider>> PhysicsEngine::checkCollide(Ref<Collider> collider) //notera att denna funktion inte tar hänsyn till om objekten redan har kolliderat
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

vector<Ref<Collider>> PhysicsEngine::rigidBodyCollide(Ref<RigidBody>& rigidBody)
{

	vector<Ref<Collider>> otherColliders;

	vector<Ref<Collider>> rigidBodyColliders = rigidBody->getGameObject()->getMultipleComponentType<Collider>(Component::ComponentEnum::COLLIDER);
	assert(rigidBodyColliders.size() < 2); //vi supportar inte flera colliders just nu

	if (rigidBodyColliders.size() == 1)
	{
		otherColliders = checkCollide(rigidBodyColliders[0]);
	}
	return otherColliders;
}

void PhysicsEngine::simulate(Ref<RigidBody> rigidBody)
{
	m_timer.start();
	m_deltaTime += m_timer.dt();
	while (m_timeStep <= m_deltaTime)
	{
		

		/*vector<Ref<Collider>> otherColliders = rigidBodyCollide(rigidBody);
		for (auto& other : otherColliders)
		{
			rigidBody->m_transform->getPosition();
		}*/

		calcPos(rigidBody);

		m_deltaTime -= m_timeStep;
	}
	m_timer.stop();
}


 Vector3 PhysicsEngine::calcPos(Ref<RigidBody>& rigidBody)
{
	rigidBody->m_acceleration = rigidBody->m_force / rigidBody->m_mass;
	rigidBody->m_acceleration.y -= rigidBody->m_gravity;

	rigidBody->m_velocity += rigidBody->m_acceleration * (float)m_timeStep;

	return rigidBody->m_transform->getPosition() + rigidBody->m_velocity * (float)m_timeStep;
}
