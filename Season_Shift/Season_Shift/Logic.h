#pragma once
#include "Component.h"
#include "GameObject.h"

class Logic : public Component
{

public:
	Logic();
	~Logic();
	virtual void start();
	virtual void update();
	virtual void onCollision();
};

