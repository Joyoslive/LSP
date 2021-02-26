#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class PhysicsEngine;

class Collider : public Component
{
	friend PhysicsEngine;
private:
	bool m_trigger;


public:
	Collider();
	virtual ~Collider();
	virtual bool collide(const Ref<Collider>& collider) = 0;
	void SetTriggerCollider(bool trigger);
	bool GetTrigger() const;
};

