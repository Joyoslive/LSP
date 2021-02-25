#include "SceneManager.h"
#include "Scenes/Scene1.h"
#include "Scenes/Scene2.h"
#include "Scenes/Scene3.h"
#include "Scenes/Scene4.h"
#include "Scenes/Scene5.h"

#include <assert.h>

SceneManager::SceneManager(Graphics *graphics)
{
	createScenes(graphics);
}

SceneManager::~SceneManager()
{
	for (int i = 0; i < m_scenes.size(); ++i)
	{
		m_scenes[i]->emptyScene();
	}
	m_scenes.clear();
	m_activeScene = nullptr;
}

void SceneManager::createScenes(Graphics* graphics)
{
	Ref<Scene> scene = std::make_shared<Scene1>(graphics);
	addScene(scene);
	Ref<Scene> scene2 = std::make_shared<Scene2>(graphics);
	addScene(scene2);
	Ref<Scene> scene3 = std::make_shared<Scene3>(graphics);
	addScene(scene3);
	Ref<Scene> scene4 = std::make_shared<Scene4>(graphics);
	addScene(scene4);
	Ref<Scene> scene5 = std::make_shared<Scene5>(graphics);
	addScene(scene5);
	setActiveScene(scene);
	//setActiveScene(scene2);
}

void SceneManager::setActiveScene(Ref<Scene> newActiveScene)
{
	m_activeScene = newActiveScene;
	m_activeScene->setUpScene();
	//Call start on all GameObjects in Scene
	m_activeScene->start();

	updateObservers();
}

void SceneManager::addScene(Ref<Scene> newScene)
{
	m_scenes.push_back(newScene);
}

Ref<Scene> SceneManager::getActiveScene() const
{
	return m_activeScene;
}

void SceneManager::changeScene(const int& sceneIndex)
{
	if (sceneIndex > m_scenes.size() - 1)
		assert(false);

	Ref<Scene> newActiveScene = m_scenes[sceneIndex];
	m_activeScene->emptyScene();

	setActiveScene(newActiveScene);
}

void SceneManager::updateActiveScene() const
{
	m_activeScene->update();
}

void SceneManager::addObserver(Ref<SceneManagerObserver> observer)
{
	m_observers.push_back(observer);
	observer->updateScene(m_activeScene);
}

void SceneManager::removeObserver(Ref<SceneManagerObserver> observer)
{
	for (int i = 0; i < m_observers.size(); ++i)
	{
		if (observer == m_observers[i])
		{
			m_observers.erase(m_observers.begin() + i);
			break;
		}
	}
}

void SceneManager::updateObservers()
{
	for (int i = 0; i < m_observers.size(); ++i)
	{
		m_observers[i]->updateScene(m_activeScene);
	}
}