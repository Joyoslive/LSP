#pragma once
#include "../Scene.h"

class Graphics;

class Scene2 : public Scene
{
public:
	Scene2(Graphics* graphics);
	~Scene2();
	void setUpScene() override;
};

