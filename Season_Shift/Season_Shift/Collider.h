#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Collider : public Component
{
private:

protected:
	/*union AbstractCollider
	{
		DirectX::BoundingSphere m_sphere;
		DirectX::BoundingOrientedBox m_obb;
	};*/

	//AbstractCollider m_boundingVolume;
public:
	Collider();
	virtual ~Collider();

	//virtual void update() = 0;
	virtual bool collide(Ref<Collider> collider) = 0;


	//bool collide(Ref<Collider> collider);

	
};

