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
	m_viewMatrix = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}



void Camera::calculateProjectionMatrix()
{
	m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
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
}