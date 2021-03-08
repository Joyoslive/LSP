#include "pch.h"
#include "PhysicsEngine.h"

#include "SphereCollider.h"
#include "OrientedBoxCollider.h"
#include "RigidBody.h"
#include "Logic.h"
#include "Transform.h"
#include <assert.h>
#include "Logger.h"
#include "CapsuleCollider.h"



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
		Ref<Collider> other = go->getComponentType<Collider>(Component::ComponentEnum::COLLIDER); //NOTE THAT A GAME OBJECT CAN HAVE MULTIPLE COLLIDERS
		if (other != nullptr)
		{
			if (collider.get() == other.get()) continue; //skip if to avoid self collision check
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
		else if (go->getComponentType<Logic>(Component::ComponentEnum::LOGIC) != nullptr && go->getComponentType<Collider>(Component::ComponentEnum::COLLIDER) != nullptr)
		{
			//check collision for non rigidbody gameobjects that have a logic component needing collision
			checkCollide(go->getComponentType<Collider>(Component::ComponentEnum::COLLIDER));
		}
	}
}

void PhysicsEngine::internalSimulate(const Ref<RigidBody>& rigidBody, long double dt)
{
	m_deltaTime += dt;
	while (m_timeStep < m_deltaTime)
	{
		rigidBody->getTransform()->setPosition(calcPos(rigidBody));

		Ref<Collider> rigidBodyCollider = rigidBody->getGameObject()->getComponentType<Collider>(Component::ComponentEnum::COLLIDER);
		if (rigidBodyCollider != nullptr)
		{
			vector<Ref<Collider>> otherColliders = checkCollide(rigidBodyCollider);
			if (!rigidBodyCollider->m_trigger)
			{
				for (auto& other : otherColliders)
				{
					if (!other->m_trigger)
					{
						collisionResponse(rigidBody, rigidBodyCollider, other);
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
	//rigidBody->m_acceleration.y -= 70;



	//pendelMotion
	if (rigidBody->m_pendelMotion)
	{
		Vector3 pos = rigidBody->getTransform()->getPosition();
		Vector3 vel = rigidBody->m_velocity;
		Vector3 acc = rigidBody->m_acceleration;

		Vector3 newVel = vel + acc * (float)m_timeStep; //use next velocity???
		Vector3 newPos = pos + newVel * (float)m_timeStep;

		Vector3 pendelDir = rigidBody->m_pendelPoint - newPos;
		pendelDir.Normalize();

		//check if rope is streached, else don't to shit
		if ((newPos - rigidBody->m_pendelPoint).Length() > rigidBody->m_pendelLength)
		{
			Vector3 correctedPos = rigidBody->m_pendelPoint - pendelDir * rigidBody->m_pendelLength; //clamp pos to inside rope radius
			Vector3 correctedVel = (correctedPos - pos) / (float)m_timeStep; //solve new vel and acc to move into corrected pos
			Vector3 correctedAcc = (correctedVel - vel) / (float)m_timeStep;
			rigidBody->m_acceleration = correctedAcc;
		}
	}
	rigidBody->m_velocity += rigidBody->m_acceleration * (float)m_timeStep;
	return rigidBody->m_transform->getPosition() + rigidBody->m_velocity * (float)m_timeStep;
}

void PhysicsEngine::collisionResponse(const Ref<RigidBody>& rigidBody, const Ref<Collider>& rigidBodyCollider, const Ref<Collider>& obb)
{
	if ((int)(obb->getType() & Component::ComponentEnum::ORIENTED_BOX_COLLIDER) == 0)
	{
		Logger::getLogger().debugLog("Can't respond to non obb collision\n");
		return;
	}
	Vector3 normal, velocity;
	if ((int)(rigidBodyCollider->getType() & Component::ComponentEnum::CAPSULE_COLLIDER))
	{
		normal = capsuleCollideObb(rigidBodyCollider, obb); //använder bara infor från den senaste krocken SÅ DET FUNKAR INTE OM MAN KOLLIDERAR MED FLERA SA
	}
	else if ((int)(rigidBodyCollider->getType() & Component::ComponentEnum::SPHERE_COLLIDER))
	{
		normal = sphereCollideObb(rigidBodyCollider, obb);
	}
	//project velocity on plane normal to collision
	velocity = rigidBody->getVelocity();
	if (normal.Dot(velocity) < 0)
	{
		velocity = velocity - (normal.Dot(velocity)) * normal;
		rigidBody->stop();
		rigidBody->m_velocity = velocity;
	}
}


Vector3 PhysicsEngine::sphereCollideObb(const Ref<Collider>& sphere, const Ref<Collider>& obb)
{
	Vector3 normal = Vector3(0, 1, 0);
	Vector3 position = std::dynamic_pointer_cast<OrientedBoxCollider>(obb)->closestPointOnObb(
		std::dynamic_pointer_cast<SphereCollider>(sphere)->getInternalCollider().Center, normal);

	sphere->getTransform()->setPosition((position + normal * 1.001f * std::dynamic_pointer_cast<SphereCollider>(sphere)->getInternalCollider().Radius));
	std::dynamic_pointer_cast<SphereCollider>(sphere)->update();
	return normal;
}

Vector3 PhysicsEngine::capsuleCollideObb(const Ref<Collider>& capsule, const Ref<Collider>& obb)
{
	auto collisionInfo = std::dynamic_pointer_cast<CapsuleCollider>(capsule)->m_collisionInfo;
	Vector3 normal = collisionInfo.m_normal;
	Vector3 position = capsule->getTransform()->getPosition();

	capsule->getTransform()->setPosition(position + normal * collisionInfo.m_penetration * 1.001f);
	std::dynamic_pointer_cast<CapsuleCollider>(capsule)->update();
	return collisionInfo.m_normal;
}