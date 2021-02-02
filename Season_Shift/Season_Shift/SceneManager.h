#pragma once
#include "Scene.h"
class SceneManager
{
private:
	std::vector<Ref<Scene>> scenes;
public:
	SceneManager();
	~SceneManager();

	void addScene();
	Ref<Scene> getScene() const;
};

