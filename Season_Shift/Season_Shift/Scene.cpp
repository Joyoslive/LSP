#include "Scene.h"

using namespace DirectX::SimpleMath;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::setUpScene()
{
	createGameObject();
	createGameObject("GameObject1");
	Ref<GameObject> gObj = createGameObject("GameObject2", Vector3(12, 4, 6));
	gObj->AddComponent(std::make_shared<RigidBodyComponent>());
	Ref<Logic> test = std::make_shared<Test>();
	gObj->AddComponent(test);
	//destroyGameObject(gObj);
}

void Scene::start()
{
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		m_sceneGameObjects[i]->start();
	}
}

void Scene::update()
{
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		m_sceneGameObjects[i]->update();
	}
}

void Scene::addGameObject(Ref<GameObject> gameObject)
{
	m_sceneGameObjects.push_back(gameObject);
}

void Scene::removeGameObject(Ref<GameObject> gameObject)
{
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		if (gameObject == m_sceneGameObjects[i])
		{
			m_sceneGameObjects.erase(m_sceneGameObjects.begin() + i);
			break;
		}
	}
}

Ref<GameObject> Scene::createGameObject(std::string gameObjectName, Vector3 position, Vector3 scale, Vector3 rotation)
{
	if (gameObjectName == "")
		gameObjectName = "Unnamed GameObject";

	Ref<GameObject> gameObject = std::make_shared<GameObject>(gameObjectName);
	Ref<Transform> transform = std::make_shared<Transform>(position, scale, rotation);
	
	gameObject->AddComponent(transform);

	addGameObject(gameObject);

	return gameObject;
}

void Scene::destroyGameObject(Ref<GameObject> destroyGameObject)
{
	removeGameObject(destroyGameObject);
	destroyGameObject->~GameObject();
}
