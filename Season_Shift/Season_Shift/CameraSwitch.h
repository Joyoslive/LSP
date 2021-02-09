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
	void Init(DebugCamera*, Ref<GameObject>, Ref<Camera>, SceneManager*, Graphics*);
	~CameraSwitch();
	void update();
private:
	SceneManager* m_sceneManager;
	DebugCamera* m_debugCamera;
	Ref<GameObject> gameObject;
	Ref<Camera> m_cam;
	Graphics* m_gph;
	bool m_cameraCheck;
};