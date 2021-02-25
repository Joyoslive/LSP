#include "Scene.h"
#include "GameObject.h"
#include "Graphics/Graphics.h"

using namespace DirectX::SimpleMath;

Scene::Scene(Graphics *graphics)
{
	m_graphics = graphics;
}

Scene::~Scene()
{

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
	m_sceneModels.clear();
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
	// We may have to order the update around! It may matter what order they are updated **********
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

void Scene::updateSceneModels()
{
	int modelSize = m_sceneModels.size();
	int gameObjectSize = m_sceneGameObjects.size();
	int modelIndex = 0;
	//Check every gameObject to check if it has an model
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		Ref<Model> model = m_sceneGameObjects[i]->getComponentType<Model>(Component::ComponentEnum::MODEL);
		//Need to find model and GameObject needs to be visible
		if (model != nullptr && m_sceneGameObjects[i]->getIsVisible())
		{
			//Just replace old model with new and if the vector is too small than we need to push the model to the model vector
			modelSize = m_sceneModels.size();
			if (modelSize - 1 >= modelIndex)
			{
				m_sceneModels[modelIndex] = model;
			}
			else if (modelSize - 1 < modelIndex)
			{
				m_sceneModels.push_back(model);
			}
			++modelIndex;
		}
	}
	modelSize = m_sceneModels.size();
	//Removes models in the model vector if the model vector is too big
	for (int i = 0; i < modelSize - modelIndex; ++i)
	{
		m_sceneModels.pop_back();
	}
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

Ref<GameObject> Scene::getGameObject(const std::string& gameObjectName)
{
	for (int i = 0; i < m_sceneGameObjects.size(); ++i)
	{
		if (m_sceneGameObjects[i]->getName() == gameObjectName)
		{
			return m_sceneGameObjects[i];
		}
	}
	return nullptr;
}

std::vector<Ref<Model>>& Scene::getSceneModels()
{
	updateSceneModels();
	return m_sceneModels;
}

std::vector<Ref<GameObject>>& Scene::getSceneGameObjects()
{
	return m_sceneGameObjects;
}
