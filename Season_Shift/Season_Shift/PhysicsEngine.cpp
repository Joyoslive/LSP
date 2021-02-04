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


Vector3 PhysicsEngine::closestPointOnObb(Vector3 point, Ref<OrientedBoxCollider> obb, Vector3& returnNormal) const
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

	//if point is inside obb move to boundery
	
	if (distance.x < obb->m_obb.Extents.x && distance.x > -obb->m_obb.Extents.x
		&& distance.y < obb->m_obb.Extents.y && distance.y > -obb->m_obb.Extents.y
		&& distance.z < obb->m_obb.Extents.z && distance.z > -obb->m_obb.Extents.z)
	{
		float xDiff = 0.0, yDiff = 0.0, zDiff = 0.0;

		if (distance.x > 0)
			xDiff = obb->m_obb.Extents.x - distance.x;
		else
			xDiff = obb->m_obb.Extents.x + distance.x;

		if (distance.y > 0)
			yDiff = obb->m_obb.Extents.y - distance.y;
		else
			yDiff = obb->m_obb.Extents.y + distance.y;

		if (distance.z > 0)
			zDiff = obb->m_obb.Extents.z - distance.z;
		else
			zDiff = obb->m_obb.Extents.z + distance.z;


		if (xDiff <= yDiff && xDiff <= zDiff)
		{
			if (distance.x < 0)
				distance.x = -obb->m_obb.Extents.x;
			else
				distance.x = obb->m_obb.Extents.x;
		}
		else if(zDiff <= yDiff)
		{
			if (distance.z < 0)
				distance.z = -obb->m_obb.Extents.z;
			else
				distance.z = obb->m_obb.Extents.z;
		}
		else
		{
			if (distance.y < 0)
				distance.y = -obb->m_obb.Extents.y;
			else
				distance.y = obb->m_obb.Extents.y;
		}

	}

	Vector3 closestPoint = obb->m_obb.Center;
	closestPoint += distance.x * unitX;
	closestPoint += distance.y * unitY;
	closestPoint += distance.z * unitZ;

	//get the normal
	// denna kod är typ baserad på kod från raytracing uppgiften i 3d-prog och jag har ingen aning om det fungerar
	float epsilon = 0.001f;

	if (abs(unitX.Dot(closestPoint - (obb->m_obb.Center + unitX * obb->m_obb.Extents.x))) < epsilon) returnNormal = unitX;
	if (abs(unitX.Dot(closestPoint - (obb->m_obb.Center - unitX * obb->m_obb.Extents.x))) < epsilon) returnNormal = unitX * -1;
	if (abs(unitY.Dot(closestPoint - (obb->m_obb.Center + unitY * obb->m_obb.Extents.y))) < epsilon) returnNormal = unitY;
	if (abs(unitY.Dot(closestPoint - (obb->m_obb.Center - unitY * obb->m_obb.Extents.y))) < epsilon) returnNormal = unitY * -1;
	if (abs(unitZ.Dot(closestPoint - (obb->m_obb.Center + unitZ * obb->m_obb.Extents.z))) < epsilon) returnNormal = unitZ;
	if (abs(unitZ.Dot(closestPoint - (obb->m_obb.Center - unitZ * obb->m_obb.Extents.z))) < epsilon) returnNormal = unitZ * -1;


	return closestPoint;
}