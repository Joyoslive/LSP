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
	gObj->AddComponent(std::make_shared<RigidBody>());
	Ref<Test> test = std::make_shared<Test>();
	gObj->AddComponent(test);
	Ref<Logic> logic = gObj->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
	//destroyGameObject(gObj);

	Ref<GameObject> go1 = createGameObject("colliderTest1", Vector3(5,0,0));
	go1->AddComponent(std::make_shared<SphereCollider>(2.0f));

	/*Ref<GameObject> go2 = createGameObject("colliderTest1", Vector3(3, 0, 0));
	go2->AddComponent(std::make_shared<SphereCollider>(2.0f));*/


	

}

void Scene::resetScene()
{
	emptyScene();
	setUpScene();
}

//Removes all GameObjects from the Scene
void Scene::emptyScene()
{
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		destroyGameObject(m_sceneGameObjects[i]);
		--i;
	}
	//m_sceneGameObjects.
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
	destroyGameObject->clearGameObject();
}

std::vector<Ref<GameObject>>& Scene::getSceneGameObjects()
{
	return m_sceneGameObjects;
}
