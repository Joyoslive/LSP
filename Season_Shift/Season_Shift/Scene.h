#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"
//#include "GameObject.h"

//#include "Graphics/Graphics.h"
//#include "Player.h"
//#include "CameraComponent.h"
//#include "PlayerJumpTrigger.h"
//#include "Rotate.h"
//#include "RigidBody.h"
//#include "Transform.h"
//#include "Logic.h"
//#include "Collider.h"
//#include "SphereCollider.h"
//#include "OrientedBoxCollider.h"
//#include "CapsuleCollider.h"
//#include "Sound.h"
//#include "Graphics/Model.h"

class GameObject;
class Graphics;
class Model;
class InGameMenu;

class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector<Ref<GameObject>> m_sceneGameObjects;

	std::vector<Ref<Model>> m_sceneNullModels;
	std::vector<Ref<Model>> m_sceneModels;

	// Pause menu
	bool m_isPaused;
	std::shared_ptr<InGameMenu> m_menu;

protected:
	Graphics* m_graphics;


private:
	void addGameObject(Ref<GameObject> gameObject);
	void removeGameObject(Ref<GameObject> gameObject);
	void updateSceneModels();
	void updateMenu();

public:
	Scene(Graphics *graphics);
	~Scene();
	void setMenu(std::shared_ptr<InGameMenu> menu);
	virtual void setUpScene() = 0;
	void resetScene();
	void emptyScene();
	void start();
	void update();
	Ref<GameObject> createGameObject(std::string gameObjectName = "", DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3(0, 0, 0)
		, DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1, 1, 1)
		, DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3(0, 0, 0));
	void destroyGameObject(Ref<GameObject> destroyGameObject);

	bool isPaused() const;

	Ref<GameObject> getGameObject(const std::string& gameObjectName);
	std::vector<Ref<Model>>& getSceneModels();
	std::vector<Ref<GameObject>>& getSceneGameObjects();
	Graphics* getGraphics() const;
};

