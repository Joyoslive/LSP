#include "DebugCamera.h"

using namespace DirectX; // For multiplying speed

DebugCamera::DebugCamera(HWND wndHandle) {
	mousePos = { 0.0f, 0.0f };
	forward = { 0.0f, 0.0f, 1.0f, 0.0f };
	right = { 1.0f, 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	speed = 5.0f;
	position = { 0.0f, 0.0f, 0.0f, 0.0f };
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	input.Init(wndHandle);
	camera.setPosition(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position));
	camera.setRotation(pitch, roll, yaw);
}

DebugCamera::~DebugCamera() {

}

void DebugCamera::Move() {
	if (input.KeyBeingPressed(Input::W)) {
		position += speed * forward;
	}
	if (input.KeyBeingPressed(Input::S)) {
		position -= speed * forward;
	}
	if (input.KeyBeingPressed(Input::A)) {
		position -= speed * right;
	}
	if (input.KeyBeingPressed(Input::D)) {
		position += speed * right;
	}
	if (input.KeyBeingPressed(Input::Shift)) {
		position -= speed * up;
	}
	if (input.KeyBeingPressed(Input::Space)) {
		position += speed * up;
	}
	camera.setPosition(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position));
}

void DebugCamera::Rotate() {
	//Get the Mouse Coordinates
	mousePos = input.MousePos();
	if (mousePos.y < 1000 && mousePos.y > -1000)
		pitch = mousePos.y * 0.1f;
	roll = mousePos.x * 0.1f;
	camera.setRotation(pitch, roll, yaw);

	DirectX::XMVECTOR defaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR defaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX rotMatrixY;

	// sets the rotation matrix
	rotMatrixY = DirectX::XMMatrixRotationY(yaw);

	//update camera based on rotation
	right = DirectX::XMVector3TransformCoord(defaultRight, rotMatrixY);
	forward = DirectX::XMVector3TransformCoord(defaultForward, rotMatrixY);
}