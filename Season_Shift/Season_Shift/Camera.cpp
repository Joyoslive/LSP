#include "pch.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include <stdexcept>

using namespace DirectX;

Camera::Camera(float x, float y, float z, int width, int height, float nearPlane, float farPlane)
{
	m_width = width;
	m_height = height;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

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

void Camera::resetCamera(bool pos, bool rot, bool proj)
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
		m_forward = XMLoadFloat3(&f);

		f.z = 0;
		f.x = 1;
		m_right = XMLoadFloat3(&f);
	}
	if (proj)
	{
		m_fieldOfView = XM_PI / 4.0;
		m_aspectRatio = (float)m_width / (float)m_height;
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
	m_orthographicMatrix = XMMatrixOrthographicLH((float)m_width, (float)m_height, m_nearPlane, m_farPlane);
}

void Camera::rotateAroundAxis(DirectX::XMVECTOR axis, float angle)
{
	auto rotationMatrix = XMMatrixRotationAxis(axis, angle);

	m_up = XMVector3Transform(m_up, rotationMatrix);
	m_right = XMVector3Transform(m_right, rotationMatrix);
	m_direction = XMVector3Transform(m_direction, rotationMatrix);
	m_forward = XMVector3Normalize(XMVectorSetY(m_direction, 0));

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
	calculateViewMatrix();
}

void Camera::setRotation(float roll, float pitch, float yaw)
{
	resetCamera(false, true, false);
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	m_up = XMVector3Transform(m_up, rotationMatrix);
	m_right = XMVector3Transform(m_right, rotationMatrix);
	m_direction = XMVector3Transform(m_direction, rotationMatrix);
	m_forward = XMVector3Normalize(XMVectorSetY(m_direction, 0));

	calculateViewMatrix();
}


void Camera::setRotationFree(float roll, float pitch, float yaw)
{
	resetCamera(false, true, false);
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	m_up = XMVector3Transform(m_up, rotationMatrix);
	m_right = XMVector3Transform(m_right, rotationMatrix);
	m_direction = XMVector3Transform(m_direction, rotationMatrix);
	m_forward = XMVector3Normalize(m_direction);

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
			axisVector = m_forward;
			break;
		case LOOK_DIR:
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

void Camera::setOffset(float x, float y, float z)
{
	m_attachedOffset = XMFLOAT3(x, y, z);
}

void Camera::updatePosition()
{
	if (!m_attachedTo)
		return;
	auto pos_f = m_attachedTo->getTransform()->getPosition() + m_attachedOffset;
	setPosition(pos_f.x, pos_f.y, pos_f.z);
}

void Camera::setFieldOfView(const float& fov)
{
	resetCamera(false, false, true);
	m_fieldOfView = fov;
	calculateProjectionMatrix();
}

const float& Camera::getFieldOfView() const
{
	return m_fieldOfView;
}

FXMVECTOR Camera::getPosition() {
	return m_position;
}

FXMVECTOR Camera::getUp()
{
	return m_up;
}

FXMVECTOR Camera::getRight()
{
	return m_right;
}

FXMVECTOR Camera::getForward()
{
	return m_forward;
}
FXMVECTOR Camera::getLookDirection()
{
	return m_direction;
}

FXMVECTOR Camera::getGlobalUp()
{
	return m_globalUp;
}

float Camera::getNearPlane()
{
	return m_nearPlane;
}

float Camera::getFarPlane()
{
	return m_farPlane;
}

CXMMATRIX Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

CXMMATRIX Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

CXMMATRIX Camera::getOrthographicMatrix() const
{
	return m_orthographicMatrix;
}
