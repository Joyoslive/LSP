#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Collider : public Component
{
private:



public:
	Collider();
	virtual ~Collider();
	virtual void update() = 0;
	virtual bool collide(Ref<Collider> collider) = 0;	
};

