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
	Vector3 velocity = m_rb->getVelocity();

	Vector3 cameraForward = m_playerCamera->getForward();
	Vector3 cameraRight = m_playerCamera->getRight();

	if (Input::getInput().keyBeingPressed(Input::W))
	{
		velocity += cameraForward * 2;
	}
	if (Input::getInput().keyBeingPressed(Input::S))
	{
		velocity -= cameraForward * 2;
	}
	if (Input::getInput().keyBeingPressed(Input::A))
	{
		velocity -= cameraRight * 2;
	}
	if (Input::getInput().keyBeingPressed(Input::D))
	{
		velocity += cameraRight * 2;
	}
	if (Input::getInput().keyPressed(Input::Esc))
	{
		exit(0);
	}
	if (Input::getInput().keyPressed(Input::Space))
	{
		velocity += Vector3(0, 10, 0);
	}
	if (Input::getInput().keyPressed(Input::Shift))
	{
		//m_rb->addForce(Vector3(0, -50, 0));
	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	
	if (velocity.Length() > 4.0f)
	{
		Vector3 velocityNormal = velocity;
		velocityNormal.Normalize();
		velocity = velocityNormal * 4.0f;
	}

	m_rb->setVelocity(velocity);
	m_playerCamera->update();
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	
 }

 void Player::lookAround() {
	 Input::getInput().mouseMovement(m_pitch, m_yaw);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }