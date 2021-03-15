#pragma once
#include "Input.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "DebugCamera.h"
#include "CameraComponent.h"
#include "SceneManagerObserver.h"

class CameraSwitch : public SceneManagerObserver
{
public:
	CameraSwitch();
	void Init(DebugCamera* dc, Ref<GameObject> go, Ref<Camera> cam);
	~CameraSwitch();
	void update(long double dt);
	Ref<Camera> getCamera() const;
	void updateScene(Scene* activeScene);
private:
	SceneManager* m_sceneManager;
	DebugCamera* m_debugCamera;
	Ref<GameObject> gameObject;
	Ref<Camera> m_cam;
	Ref<Camera> m_mainCamera;
	Graphics* m_gph;
	bool m_cameraCheck;
};