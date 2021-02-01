#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	resetCamera();
	
	m_viewMatrix = {};
	m_projectionMatrix = {};
	m_orthographicMatrix = {};
	calculateViewMatrix();
	calculateProjectionMatrix();
	calculateOrthographicMatrix();
}


Camera::Camera(float x, float y, float z)
{
	resetCamera();
	setPosition(x, y, z);

	m_viewMatrix = {};
	m_projectionMatrix = {};
	m_orthographicMatrix = {};
	calculateViewMatrix();
	calculateProjectionMatrix();
	calculateOrthographicMatrix();
}


Camera::~Camera() {}

void Camera::resetCamera()
{
	auto f = XMFLOAT3(0, 0, 0);
	m_position = XMLoadFloat3(&f);
	m_rotation = XMLoadFloat3(&f);

	f.y = 1;
	m_globalUp = XMLoadFloat3(&f);
	m_up = XMLoadFloat3(&f);

	f.y = 0;
	f.z = 1;
	m_direction = XMLoadFloat3(&f);
	m_lookAt = XMLoadFloat3(&f);

	f.z = 0;
	f.x = -1;
	m_right = XMLoadFloat3(&f);


	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;
	m_fieldOfView = XM_PI / 4;
	m_aspectRatio = 1280 / 720;
}

void Camera::calculateViewMatrix()
{
	calculateLookAt();
	m_viewMatrix = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}



void Camera::calculateProjectionMatrix()
{
	m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::calculateLookAt()
{
	m_lookAt = XMVectorAdd(m_position, m_direction);
}

void Camera::calculateOrthographicMatrix()
{
	int width = 1280;
	int height = 720;
	m_orthographicMatrix = XMMatrixOrthographicLH((float)width, (float)height, m_nearPlane, m_farPlane);
}

void Camera::setPosition(float x, float y, float z)
{
	auto pos = XMFLOAT3(x, y, z);
	m_position = XMLoadFloat3(&pos);
	
	calculateViewMatrix();
}

void Camera::setRotation(float roll, float pitch, float yaw)
{
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	auto baseUp = XMFLOAT3(0, 1, 0);
	m_up = XMLoadFloat3(&baseUp);

	auto baseRight = XMFLOAT3(-1, 0, 0);
	m_right = XMLoadFloat3(&baseRight);

	auto baseDirection = XMFLOAT3(0, 0, 1);
	m_direction = XMLoadFloat3(&baseDirection);

	m_up = XMVector3Transform(m_up, rotationMatrix);
	m_right = XMVector3Transform(m_right, rotationMatrix);
	m_direction = XMVector3Transform(m_direction, rotationMatrix);

	calculateViewMatrix();
}

DirectX::XMMATRIX Camera::getViewMatrix()
{
	return m_viewMatrix;
}

DirectX::XMMATRIX Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera::getOrthographicMatrix()
{
	return m_orthographicMatrix;
}
