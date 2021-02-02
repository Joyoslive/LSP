#include "SceneManager.h"

SceneManager::SceneManager()
{
	createScenes();
}

SceneManager::~SceneManager()
{

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
	Ref<Scene> newActiveScene = m_scenes[sceneIndex];
	m_activeScene->emptyScene();
	setActiveScene(newActiveScene);
	newActiveScene->start();
}