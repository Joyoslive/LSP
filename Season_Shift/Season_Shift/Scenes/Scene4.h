#pragma once
#pragma once
#include "../Scene.h"

class Graphics;

class Scene4 : public Scene
{
public:
	Scene4(Graphics* graphics);
	~Scene4();
	void setUpScene() override;
};

