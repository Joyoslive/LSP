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


Vector3 PhysicsEngine::closestPointOnObb(Vector3 point, Ref<OrientedBoxCollider> obb) const
{
	//från boken
	//Real time collision detection av Christer Ericson


	//obb unit vectors, fan att DirectXCollision.h bara har en quaternion i sin obb
	Vector3 unitX;
	unitX.x	= obb->m_transform->getWorldMatrix().m[0][0];
	unitX.y	= obb->m_transform->getWorldMatrix().m[0][1];
	unitX.z	= obb->m_transform->getWorldMatrix().m[0][2];
	unitX.Normalize();
	
	Vector3 unitY;
	unitY.x = obb->m_transform->getWorldMatrix().m[1][0];
	unitY.y = obb->m_transform->getWorldMatrix().m[1][1];
	unitY.z = obb->m_transform->getWorldMatrix().m[1][2];
	unitY.Normalize();

	Vector3 unitZ;
	unitZ.x = obb->m_transform->getWorldMatrix().m[2][0];
	unitZ.y = obb->m_transform->getWorldMatrix().m[2][1];
	unitZ.z = obb->m_transform->getWorldMatrix().m[2][2];
	unitZ.Normalize();

	Vector3 d = point - obb->m_obb.Center;
	
	Vector3 distance;
	distance.x = unitX.Dot(d);
	distance.y = unitY.Dot(d);
	distance.z = unitZ.Dot(d);

	if (distance.x > obb->m_obb.Extents.x) distance.x = obb->m_obb.Extents.x;
	if (distance.x < -obb->m_obb.Extents.x) distance.x = -obb->m_obb.Extents.x;

	if (distance.y > obb->m_obb.Extents.y) distance.y = obb->m_obb.Extents.y;
	if (distance.y < -obb->m_obb.Extents.y) distance.y = -obb->m_obb.Extents.y;

	if (distance.z > obb->m_obb.Extents.z) distance.z = obb->m_obb.Extents.z;
	if (distance.z < -obb->m_obb.Extents.z) distance.z = -obb->m_obb.Extents.z;

	Vector3 ClosestPoint = obb->m_obb.Center;
	ClosestPoint += distance.x * unitX;
	ClosestPoint += distance.y * unitY;
	ClosestPoint += distance.z * unitZ;

	return ClosestPoint;
}