#include "Player.h"

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_yaw = DirectX::XM_2PI/2;
	 m_pitch = 0.0f;
	 m_roll = 0.0f;
 }

 Player::~Player()
 {

 }

 void Player::start()
 {
	 m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	 m_rb = m_gameObject->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }

 void Player::update()
 {
	lookAround();
	if (Input::getInput().keyPressed(Input::W))
	{
		m_rb->addForce(Vector3(0, 0, -50));
	}
	if (Input::getInput().keyPressed(Input::S))
	{
		m_rb->addForce(Vector3(0, 0, 50));
	}
	if (Input::getInput().keyPressed(Input::Space))
	{
		m_rb->addForce(Vector3(0, 50, 0));
	}
	if (Input::getInput().keyPressed(Input::Shift))
	{
		m_rb->addForce(Vector3(0, -50, 0));
	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	m_playerCamera->update();
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	
 }

 void Player::lookAround() {
	 Input::getInput().mouseMovement(m_pitch, m_yaw);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }