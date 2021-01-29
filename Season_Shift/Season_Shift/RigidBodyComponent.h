#pragma once
#include "Component.h"

class RigidBodyComponent : Component
{
public:
	RigidBodyComponent(std::shared_ptr<GameObject> gameObject);

private:
	float mass = 54;
};

