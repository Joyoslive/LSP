#pragma once
#include "Scene.h"
#include "SceneManagerObserver.h"

class SceneManager
{
private:
	std::vector<Ref<Scene>> m_scenes;
	Ref<Scene> m_activeScene;
	std::vector<Ref<SceneManagerObserver>> m_observers;

private:
	void createScenes(Graphics *graphics);
	void addScene(Ref<Scene> newScene);
	void setActiveScene(Ref<Scene> newActiveScene);
	void updateObservers();

public:
	SceneManager(Graphics *graphics);
	~SceneManager();

	Ref<Scene> getActiveScene() const;
	void changeScene(const int& sceneIndex);
	void updateActiveScene() const;
	void addObserver(Ref<SceneManagerObserver> observer);
	void removeObserver(Ref<SceneManagerObserver> observer);
};

