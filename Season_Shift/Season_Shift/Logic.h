#pragma once
#include "Component.h"
#include "GameObject.h"

class Collider;

class Logic : public Component
{

public:
	Logic();
	~Logic();
	virtual void start();
	virtual void update();
	virtual void onCollision(Ref<Collider> collider);
};

class Test : public Logic
{
public:
	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
};

