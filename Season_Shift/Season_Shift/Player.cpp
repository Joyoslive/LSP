#include "Player.h"

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_yaw = DirectX::XM_2PI/2;
	 m_pitch = 0.0f;
	 m_roll = 0.0f;
	 respawn = { 0, 0, 0 };
 }

 Player::~Player()
 {

 }

 void Player::start()
 {
	 m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	 m_playerCamera->setOffset(0, 3.0f, -0.5);
	 m_rb = m_gameObject->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }	

 void Player::update()
 {
	lookAround();
	
	Vector3 velocity = m_rb->getVelocity();
	Vector3 cameraForward = m_playerCamera->getForward();
	Vector3 cameraRight = m_playerCamera->getRight();
	detectDeath(-35.0f);

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
		velocity -= Vector3(0, 50, 0);
		//m_rb->addForce(Vector3(0, -50, 0));
	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;
	if (velocitySkipY.Length() > 0.1f)
	{
		OutputDebugStringA("Hej");
		Vector3 velocityNormal = velocitySkipY;
		velocityNormal.Normalize();
		velocitySkipY -= velocityNormal * 0.1f;
	}

	//velocitySkipY = velocity;
	velocitySkipY.y = 0;
	if (velocitySkipY.Length() > 20.0f)
	{
		Vector3 velocityNormal = velocitySkipY;
		velocityNormal.Normalize();
		velocitySkipY = velocityNormal * 20.0f;
	}
	velocitySkipY.y = velocity.y;
	velocity = velocitySkipY;

	m_rb->setVelocity(velocity);
	m_playerCamera->update();
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	
 }

 void Player::lookAround() 
 {
	 Input::getInput().mouseMovement(m_pitch, m_yaw);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
 }

 void Player::detectDeath(float death) 
 {
	 if (m_rb->getTransform()->getPosition().y < death)
	 {
		 m_rb->getTransform()->setPosition(respawn);
	 }
 }

 void Player::setRespawn(Vector3 incomingRespawn)
 {
	 respawn = incomingRespawn;
 }