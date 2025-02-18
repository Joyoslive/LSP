#pragma once
#include "Scene.h"
#include "SceneManagerObserver.h"

class InGameMenu;
class ResultMenu;
class Window;

class SceneManager
{
private:
	std::vector<Ref<Scene>> m_scenes;
	Ref<Scene> m_activeScene;
	std::vector<Ref<SceneManagerObserver>> m_observers;

	std::shared_ptr<InGameMenu> m_menu;
	std::shared_ptr<ResultMenu> m_resultMenu;
	bool m_timeToQuit;

private:
	void createMenu(Graphics* graphics);
	void createScenes(Graphics *graphics);
	void addScene(Ref<Scene> newScene);
	void setActiveScene(Ref<Scene> newActiveScene);
	void updateObservers();

public:
	SceneManager(Graphics *graphics, const Window* const win);
	~SceneManager();

	Scene* getActiveScene() const;
	void changeScene(const int& sceneIndex);
	void updateActiveScene() const;
	void addObserver(Ref<SceneManagerObserver> observer);
	void removeObserver(Ref<SceneManagerObserver> observer);
	bool currentScenePaused() const;
	void quitGame();
	bool shouldQuit() const;
};

