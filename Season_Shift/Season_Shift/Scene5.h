#pragma once
#include "Scene.h"

class Scene5 : public Scene
{
private:
public:
	Scene5(Graphics* graphics);
	~Scene5() = default;
	void setUpScene() override;
};
