#pragma once
#include "Scene.h"
class SceneManager
{
private:
	std::vector<Ref<Scene>> m_scenes;
	Ref<Scene> m_activeScene;

private:
	void createScenes();
	void addScene(Ref<Scene> newScene);
	void setActiveScene(Ref<Scene> newActiveScene);

public:
	SceneManager();
	~SceneManager();

	Ref<Scene> getActiveScene() const;
	void changeScene(const int& sceneIndex);
	void updateActiveScene() const;
};

