#pragma once

#include "Component.h"
#include "Camera.h"
//#include "Camera.h"

class CameraComponent : public Component
{
private:
	std::shared_ptr<Camera> m_camera;
	DirectX::XMFLOAT3 m_offset;
public:
	CameraComponent();
	~CameraComponent();

	void update() override;
	void initialize() override;

	std::shared_ptr<Camera> getCamera() const;

	void setOffset(float x, float y, float z);
	void setRotation(float roll, float pitch, float yaw);
	
	void rotateAroundAxis(DirectX::XMFLOAT3 axis, float angle);
	void rotateAroundSetAxis(Camera::Axis axis, float angle);

	void setFieldOfView(const float& fov);
	const float& getFieldOfView() const;

	DirectX::CXMMATRIX getViewMatrix();
	DirectX::CXMMATRIX getProjectionMatrix();
	DirectX::CXMMATRIX getOrthographicMatrix();

	DirectX::FXMVECTOR getUp();
	DirectX::FXMVECTOR getRight();
	DirectX::FXMVECTOR getForward();
	DirectX::FXMVECTOR getLookDirection();
	DirectX::FXMVECTOR getGlobalUp();
};

