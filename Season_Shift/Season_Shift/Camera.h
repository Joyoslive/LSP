#pragma once
#include <DirectXMath.h>
#include <memory>

#include "GameObject.h"

class Camera
{
private:
	DirectX::XMVECTOR m_position;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMVECTOR m_direction;
	DirectX::XMVECTOR m_forward;
	DirectX::XMVECTOR m_lookAt;
	DirectX::XMVECTOR m_right;
	DirectX::XMVECTOR m_up;
	DirectX::XMVECTOR m_globalUp;

	DirectX::XMMATRIX m_projectionMatrix;
	float m_nearPlane, m_farPlane;
	float m_fieldOfView;
	int m_width, m_height;
	float m_aspectRatio;

	DirectX::XMMATRIX m_orthographicMatrix;

	std::shared_ptr<GameObject> m_attachedTo;
	DirectX::XMFLOAT3 m_attachedOffset;
private:
	void resetCamera(bool pos=true, bool rot=true, bool proj=true);			// Reset the camera to (0,0,0) pos and rotation 
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
		LOOK_DIR,
		GLOBAL_UP,
	};

public:
	Camera(float x=0, float y=0, float z=0, int width=1280, int height=720, float nearPlane=0.1, float farPlane=1000);
	~Camera();

	

	void setPosition(float x, float y, float z);
	void setPosition(DirectX::CXMVECTOR position);
	void setRotation(float roll, float pitch, float yaw);
	void rotateAroundAxis(DirectX::XMFLOAT3 axis, float angle);
	void rotateAroundSetAxis(Axis axis, float angle);

	void attachTo(std::shared_ptr<GameObject> gameObject, float offsetX=0, float offsetY=0, float offsetZ=0);
	void setOffset(float x, float y, float z);
	void updatePosition();

	DirectX::CXMMATRIX getViewMatrix() const;
	DirectX::CXMMATRIX getProjectionMatrix() const;
	DirectX::CXMMATRIX getOrthographicMatrix() const;
	DirectX::FXMVECTOR getPosition();
	DirectX::FXMVECTOR getUp();
	DirectX::FXMVECTOR getRight();
	DirectX::FXMVECTOR getForward();
	DirectX::FXMVECTOR getLookDirection();
	DirectX::FXMVECTOR getGlobalUp();
};

