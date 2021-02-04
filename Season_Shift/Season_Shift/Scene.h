#pragma once
#include <vector>
#include "GameObject.h"
#include "Graphics/Graphics.h"

class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector<Ref<GameObject>> m_sceneGameObjects;
	Graphics* m_graphics; 
private:
	void addGameObject(Ref<GameObject> gameObject);
	void removeGameObject(Ref<GameObject> gameObject);
public:
	Scene(Graphics* graphics);
	~Scene();
	void setUpScene();
	void resetScene();
	void emptyScene();
	void start();
	void update();
	Ref<GameObject> createGameObject(std::string gameObjectName = "", DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3(0, 0, 0)
		, DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1, 1, 1)
		, DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3(0, 0, 0));
	void destroyGameObject(Ref<GameObject> destroyGameObject);
	Ref<std::vector<Ref<Model>>>& getSceneModels();

	std::vector<Ref<GameObject>>& getSceneGameObjects();
};

