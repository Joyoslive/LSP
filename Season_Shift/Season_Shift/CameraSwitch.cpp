#include "CameraSwitch.h"

CameraSwitch::CameraSwitch()
{
}


void CameraSwitch::Init(DebugCamera* dc, Ref<GameObject> go, Ref<Camera> cam, SceneManager* sceneManager, Graphics* gph)
{
	m_debugCamera = dc;
	gameObject = go; 
	m_cameraCheck = false;
	m_cam = cam;
	m_sceneManager = sceneManager;
	m_gph = gph;
}

CameraSwitch::~CameraSwitch()
{

}

void CameraSwitch::update()
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
		m_debugCamera->move();
		m_gph->render(m_sceneManager->getActiveScene()->getSceneModels(), m_cam);
	}
	else
	{
		m_gph->render(m_sceneManager->getActiveScene()->getSceneModels(), gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera());//cam);//player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera());//cam);
	}
}