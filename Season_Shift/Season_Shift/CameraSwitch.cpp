#include "pch.h"
#include "CameraSwitch.h"
#include "GameObject.h"

CameraSwitch::CameraSwitch()
{
}


void CameraSwitch::Init(DebugCamera* dc, Ref<GameObject> go, Ref<Camera> cam)
{
	m_debugCamera = dc;
	gameObject = go; 
	m_cameraCheck = false;
	m_cam = cam;
}

CameraSwitch::~CameraSwitch()
{

}

void CameraSwitch::update(long double dt)
{
	if (Input::getInput().keyPressed(Input::C))
	{
		if (m_cameraCheck == true)
		{
			m_cameraCheck = false;
		}
		else
		{
			m_cameraCheck = true;
		}
	}
	if (m_cameraCheck == true)
	{
		m_debugCamera->rotate();
		m_debugCamera->move(dt);
	}
}

Ref<Camera> CameraSwitch::getCamera() const
{
	if (m_cameraCheck == true)
	{
		return m_cam;
	}
	else if (gameObject != nullptr)
	{
		return gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera();
	}
	else if (m_mainCamera != nullptr)
	{
		return m_mainCamera;
	}

	assert(false);
}

void CameraSwitch::updateScene(Scene* activeScene)
{
	gameObject = activeScene->getGameObject("player");
	m_mainCamera = activeScene->getSceneMainCamera();
}