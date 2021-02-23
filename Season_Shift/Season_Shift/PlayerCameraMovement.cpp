#include "PlayerCameraMovement.h"

void PlayerCameraMovement::start()
{
	m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
}

void PlayerCameraMovement::update()
{

}