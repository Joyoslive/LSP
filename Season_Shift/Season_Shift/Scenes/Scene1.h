#pragma once
#include "../Scene.h"

class Graphics;

class Scene1 : public Scene
{
private:

public:
	Scene1(Graphics* graphics);
	~Scene1();
	void setUpScene() override;
};

