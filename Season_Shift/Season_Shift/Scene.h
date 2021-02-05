#pragma once
#include <vector>
#include "GameObject.h"

class Graphics;

class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector<Ref<GameObject>> m_sceneGameObjects;
	std::vector<Ref<Model>> m_sceneModels;
	Graphics* m_graphics;
private:
	void addGameObject(Ref<GameObject> gameObject);
	void removeGameObject(Ref<GameObject> gameObject);
	void updateSceneModels();
public:
	Scene(Graphics *graphics);
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

	Ref<GameObject> getGameObject(const std::string& gameObjectName);
	std::vector<Ref<Model>>& getSceneModels();
	std::vector<Ref<GameObject>>& getSceneGameObjects();
};

