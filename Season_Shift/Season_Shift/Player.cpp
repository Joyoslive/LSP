#include "Player.h"

using namespace DirectX::SimpleMath;

 Player::Player(const Ref<Input>& input)
 {
	 m_input = input;
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
	m_input->update();
	if (m_input->keyPressed(Input::W))
	{
		m_rb->addForce(Vector3(0, 0, -50));
	}
	if (m_input->keyPressed(Input::S))
	{
		m_rb->addForce(Vector3(0, 0, 50));
	}
	if (m_input->keyPressed(Input::Space))
	{
		m_rb->addForce(Vector3(0, 50, 0));
	}
	if (m_input->keyPressed(Input::Shift))
	{
		m_rb->addForce(Vector3(0, -50, 0));
	}
	if (m_input->keyPressed(Input::L))
	{
		m_input->lockMouse();
	}
	m_playerCamera->update();
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	
 }

 void Player::lookAround() {
	 m_input->mouseMovement(m_pitch, m_yaw);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }