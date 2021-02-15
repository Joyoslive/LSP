#pragma once
#include "../Scene.h"

class Graphics;

class Scene3 : public Scene
{
public:
	Scene3(Graphics* graphics);
	~Scene3();
	void setUpScene() override;
};

