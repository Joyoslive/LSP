#pragma once
#include "Component.h"

class Logic : public Component
{

public:
	Logic(std::shared_ptr<GameObject> gameObject);
	~Logic();
	virtual void start();
	virtual void update();
	virtual void onCollision();
};

/*class Test : public Logic
{
public:
	Test(std::shared_ptr<GameObject> gameObject) : Logic(gameObject)
	{

	};

	void update() override 
	{
		//OutputDebugStringW(L"Hello");
	};
};*/

