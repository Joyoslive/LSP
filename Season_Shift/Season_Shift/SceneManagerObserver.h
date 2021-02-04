#pragma once
#include "Component.h"

class Scene;

class SceneManagerObserver
{
public:
	SceneManagerObserver() = default;
	~SceneManagerObserver() = default;
	virtual void updateScene(Ref<Scene> activeScene) = 0;
};

