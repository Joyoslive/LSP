#include "SceneManager.h"

SceneManager::SceneManager()
{
	createScenes();
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

void SceneManager::createScenes()
{
	Ref<Scene> scene = std::make_shared<Scene>();
	addScene(scene);
	setActiveScene(scene);
}

void SceneManager::setActiveScene(Ref<Scene> newActiveScene)
{
	m_activeScene = newActiveScene;
	m_activeScene->setUpScene();
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
	//Call start on all GameObjects in Scene
	newActiveScene->start();
}

void SceneManager::updateActiveScene() const
{
	m_activeScene->update();
}