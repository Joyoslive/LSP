#include "Camera.h"

#include <stdexcept>

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

void Camera::resetCamera(bool pos, bool rot)
{
	auto f = XMFLOAT3(0, 0, 0);
	if (pos) {
		m_position = XMLoadFloat3(&f);
	}
	if (rot) {
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


		m_nearPlane = 0.1;
		m_farPlane = 1000.0;
		m_fieldOfView = XM_PI / 4.0;
		m_aspectRatio = (float)1280 / (float)720;
	}
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

void Camera::rotateAroundAxis(DirectX::XMVECTOR axis, float angle)
{
	auto rotationMatrix = XMMatrixRotationAxis(axis, angle);

	m_up = XMVector3Transform(m_up, rotationMatrix);
	m_right = XMVector3Transform(m_right, rotationMatrix);
	m_direction = XMVector3Transform(m_direction, rotationMatrix);

	calculateViewMatrix();
}

void Camera::setPosition(float x, float y, float z)
{
	auto pos = XMFLOAT3(x, y, z);
	m_position = XMLoadFloat3(&pos);
	
	calculateViewMatrix();
}

void Camera::setPosition(DirectX::CXMVECTOR position)
{
	m_position = position;
}

void Camera::setRotation(float roll, float pitch, float yaw)
{
	resetCamera(false, false);
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

void Camera::rotateAroundAxis(XMFLOAT3 axis, float angle)
{
	auto axisVector = XMLoadFloat3(&axis);
	rotateAroundAxis(axisVector, angle);
}

void Camera::rotateAroundSetAxis(Axis axis, float angle)
{
	XMVECTOR axisVector = {};
	switch (axis)
	{
		case UP:
			axisVector = m_up;
			break;
		case RIGHT:
			axisVector = m_right;
			break;
		case FORWARD:
			axisVector = m_direction;
			break;
		case GLOBAL_UP:
			axisVector = m_globalUp;
			break;
	}

	rotateAroundAxis(axisVector, angle);
}

void Camera::attachTo(std::shared_ptr<GameObject> gameObject, float offsetX, float offsetY, float offsetZ)
{
	if (!gameObject->getTransform())
		throw std::runtime_error("Game Object does not have a valid transform component");

	m_attachedTo = gameObject;
	m_attachedOffset = XMFLOAT3(offsetX, offsetY, offsetZ);
}

void Camera::updatePosition()
{
	if (!m_attachedTo)
		return;
	auto pos_f = m_attachedTo->getTransform()->getPosition() + m_attachedOffset;
	setPosition(pos_f.x, pos_f.y, pos_f.z);
}

DirectX::XMVECTOR Camera::getPosition() {
	return m_position;
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
