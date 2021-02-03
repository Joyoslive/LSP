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

class Test : public Logic
{
public:
	void start() override;
	void update() override;
	void onCollision() override;
};

