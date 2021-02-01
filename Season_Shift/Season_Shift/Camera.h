#pragma once
#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMVECTOR m_position;

	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMVECTOR m_direction;
	DirectX::XMVECTOR m_lookAt;
	DirectX::XMVECTOR m_right;
	DirectX::XMVECTOR m_up;
	DirectX::XMVECTOR m_globalUp;

	DirectX::XMMATRIX m_projectionMatrix;
	float m_nearPlane, m_farPlane;
	float m_fieldOfView;
	float m_aspectRatio;

	DirectX::XMMATRIX m_orthographicMatrix;
private:
	void resetCamera();			// Reset the camera to (0,0,0) pos and rotation 
	void calculateViewMatrix();
	void calculateProjectionMatrix();
	void calculateOrthographicMatrix();
	void calculateLookAt();
	void rotateAroundAxis(DirectX::XMVECTOR axis, float angle);
public:
	enum Axis
	{
		UP,
		RIGHT,
		FORWARD,
	};

public:
	Camera();
	Camera(float x, float y, float z);
	~Camera();

	void setPosition(float x, float y, float z);
	void setRotation(float roll, float pitch, float yaw);
	void rotateAroundAxis(DirectX::XMFLOAT3 axis, float angle);
	void rotateAroundSetAxis(Axis axis, float angle);

	DirectX::XMMATRIX getViewMatrix();
	DirectX::XMMATRIX getProjectionMatrix();
	DirectX::XMMATRIX getOrthographicMatrix();
};
