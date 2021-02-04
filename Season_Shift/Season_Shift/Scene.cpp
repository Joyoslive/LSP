#include "Scene.h"


using namespace DirectX::SimpleMath;

Scene::Scene(Graphics *graphics)
{
	m_graphics = graphics;
}

Scene::~Scene()
{

}

void Scene::setUpScene()
{
	Ref<Model> model = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	addModel(model);

	Ref<Model> model2 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	addModel(model2);

	createGameObject();
	createGameObject("GameObject1");
	Ref<GameObject> gObj = createGameObject("GameObject2", Vector3(12, 4, 6));
	gObj->AddComponent(std::make_shared<RigidBody>());
	Ref<Test> test = std::make_shared<Test>();
	gObj->AddComponent(test);
	Ref<Logic> logic = gObj->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
	//destroyGameObject(gObj);

	Ref<GameObject> go1 = createGameObject("colliderTest1", Vector3(1,0,4), Vector3(0.2f, 0.2f, 0.2f));
	go1->AddComponent(std::make_shared<SphereCollider>(2.0f));
	go1->AddComponent(model);

	Ref<GameObject> go2 = createGameObject("colliderTest2", Vector3(-2.0, 0, 2), Vector3(0.2f, 0.2f, 0.2f));
	go2->AddComponent(model2);



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
	clearModels();
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

void Scene::addModel(Ref<Model> model)
{
	m_sceneModels.push_back(model);
}

void Scene::clearModels()
{
	m_sceneModels.clear();
}

Ref<GameObject> Scene::createGameObject(std::string gameObjectName, Vector3 position, Vector3 scale, Vector3 rotation)
{
	if (gameObjectName == "")
		gameObjectName = "Unnamed GameObject";

	Ref<GameObject> gameObject = std::make_shared<GameObject>(gameObjectName, shared_from_this());
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

std::vector<Ref<Model>> Scene::getModels()
{
	std::vector<Ref<Model>> modelVector; 
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		Ref<Model> tempModel = m_sceneGameObjects[i]->getComponentType<Model>(Component::ComponentEnum::MODEL);
		if (tempModel != nullptr)
		{
			modelVector.push_back(tempModel);
		}
	}
	return modelVector;
}

std::vector<Ref<GameObject>>& Scene::getSceneGameObjects()
{
	return m_sceneGameObjects;
}
