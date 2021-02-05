#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	m_camera = Camera();
	m_componentType = Component::ComponentEnum::CAMERA;
	m_offset = {0, 0, 0};
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::update()
{
	m_camera.updatePosition();
}

void CameraComponent::initialize()
{
	m_camera.attachTo(m_gameObject, m_offset.x, m_offset.y, m_offset.z);
}

std::shared_ptr<Camera> CameraComponent::getCamera()
{
	return std::make_shared<Camera>(m_camera);
}

void CameraComponent::setOffset(float x, float y, float z)
{
	m_offset = {x, y, z};
	m_camera.setOffset(x, y, z);
}

void CameraComponent::setRotation(float roll, float pitch, float yaw)
{
	m_camera.setRotation(roll, pitch, yaw);
}

void CameraComponent::rotateAroundAxis(DirectX::XMFLOAT3 axis, float angle)
{
	m_camera.rotateAroundAxis(axis, angle);
}

void CameraComponent::rotateAroundSetAxis(Camera::Axis axis, float angle)
{
	m_camera.rotateAroundSetAxis(axis, angle);
}

DirectX::CXMMATRIX CameraComponent::getViewMatrix()
{
	return m_camera.getViewMatrix();
}

DirectX::CXMMATRIX CameraComponent::getProjectionMatrix()
{
	return m_camera.getProjectionMatrix();
}

DirectX::CXMMATRIX CameraComponent::getOrthographicMatrix()
{
	return m_camera.getOrthographicMatrix();
}

DirectX::FXMVECTOR CameraComponent::getUp()
{
	return m_camera.getUp();
}

DirectX::FXMVECTOR CameraComponent::getRight()
{
	return m_camera.getRight();
}

DirectX::FXMVECTOR CameraComponent::getForward()
{
	return m_camera.getForward();
}

DirectX::FXMVECTOR CameraComponent::getLookDirection()
{
	return m_camera.getLookDirection();
}

DirectX::FXMVECTOR CameraComponent::getGlobalUp()
{
	return m_camera.getGlobalUp();
}
