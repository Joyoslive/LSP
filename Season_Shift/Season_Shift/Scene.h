#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"
#include "Camera.h"
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
class SceneManager;
class InGameMenu;
class ResultMenu;

class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector<Ref<GameObject>> m_sceneGameObjects;

	std::vector<Ref<Model>> m_sceneNullModels;
	std::vector<Ref<Model>> m_sceneModels;

	// Pause menu
	bool m_isPaused;
	std::shared_ptr<InGameMenu> m_menu;
	std::shared_ptr<ResultMenu> m_resultMenu;

protected:
	Graphics* m_graphics;

	SceneManager* m_sceneManager;
	Ref<Camera> m_mainCamera;
private:
	void addGameObject(Ref<GameObject> gameObject);
	void removeGameObject(Ref<GameObject> gameObject);
	void updateSceneModels();
	void updateMenu();

public:
	Scene(Graphics *graphics, SceneManager *sm=nullptr);
	~Scene();
	void setIsPaused(bool isPaused);		// no side effects
	void setPauseState(bool isPaused);
	void setMenu(std::shared_ptr<InGameMenu> menu, std::shared_ptr<ResultMenu> resultMenu);
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
	const Ref<ResultMenu>& getResultMenu() const;

	Ref<GameObject> getGameObject(const std::string& gameObjectName);
	std::vector<Ref<Model>>& getSceneModels();
	std::vector<Ref<GameObject>>& getSceneGameObjects();
	Graphics* getGraphics() const;
	Ref<Camera> getSceneMainCamera() const;
};

