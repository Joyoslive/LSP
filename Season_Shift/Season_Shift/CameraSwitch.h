#pragma once
#include "Input.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "DebugCamera.h"
#include "CameraComponent.h"

class CameraSwitch
{
public:
	CameraSwitch();
	void Init(DebugCamera* dc, Ref<GameObject> go, Ref<Camera> cam);
	~CameraSwitch();
	void update();
	Ref<Camera> getCamera() const;
private:
	SceneManager* m_sceneManager;
	DebugCamera* m_debugCamera;
	Ref<GameObject> gameObject;
	Ref<Camera> m_cam;
	Graphics* m_gph;
	bool m_cameraCheck;
};