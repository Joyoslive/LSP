#include "Player.h"

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_yaw = DirectX::XM_2PI/2;
	 m_pitch = 0.0f;
	 m_roll = 0.0f;
	 respawn = { 0, 0, 0 };
	 m_disable = false;
	 m_frameTime = 0.0f;
	 m_speed = 5000.0f;
	 m_maxSpeed = 30.0f;
	 m_minSpeed = 0.1f;
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

	 m_rb->setGravity(15.0);
 }	

 const Vector3& Player::antiMovement(Vector3 velocity)
 {
	 const float antiMoveSize = 14.3f;
	 if (velocity.Length() > m_minSpeed)
	 {
		 Vector3 velocityNormal = velocity;
		 velocityNormal.Normalize();
		 velocity -= velocityNormal * antiMoveSize * velocity.Length() * m_frameTime;
	 }
	 return velocity;
 }

 const Vector3& Player::checkMaxSpeed(Vector3 velocity, const float& velocityY)
 {
	 if (velocity.Length() + velocityY > m_maxSpeed)
	 {
		 Vector3 velocityNormal = velocity;
		 velocityNormal.Normalize();
		 velocity = velocityNormal * m_maxSpeed;
	 }
	 return velocity;
 }

 const Vector3& Player::checkMinSpeed(const Vector3& velocity)
 {
	 if (velocity.Length() < m_minSpeed)
		 return Vector3::Zero;
	 return velocity;
 }

 void Player::update()
 {
	lookAround();
	detectDeath(-35.0f);
	Vector3 velocity = m_rb->getVelocity();
	Vector3 cameraForward = m_playerCamera->getForward();
	Vector3 cameraRight = m_playerCamera->getRight();
	Vector3 moveDirection = Vector3::Zero;
	
	if (Input::getInput().keyPressed(Input::C))
	{
		if (m_disable == false)
		{
			m_disable = true;
		}
		else
		{
			m_disable = false;
		}
	}
	if (m_disable == false)
	{
		if (Input::getInput().keyBeingPressed(Input::W))
		{
			moveDirection += cameraForward;
			//velocity += cameraForward *m_frameTime * m_speed;
		}
		if (Input::getInput().keyBeingPressed(Input::S))
		{
			moveDirection -= cameraForward;
			//velocity -= cameraForward *m_frameTime * m_speed;
		}
		if (Input::getInput().keyBeingPressed(Input::A))
		{
			moveDirection -= cameraRight;
			//velocity -= cameraRight *m_frameTime * m_speed;
		}
		if (Input::getInput().keyBeingPressed(Input::D))
		{
			moveDirection += cameraRight;
			//velocity += cameraRight *m_frameTime * m_speed;
		}
		if (Input::getInput().keyPressed(Input::Esc))
		{
			exit(0);
		}
		if (Input::getInput().keyPressed(Input::Space))
		{
			velocity += Vector3(0,  10, 0);
		}
		if (Input::getInput().keyPressed(Input::Shift))
		{
			velocity -= Vector3(0, 50, 0);
		}
	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	
	moveDirection.y = 0;
	moveDirection.Normalize();

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;
	velocitySkipY += moveDirection * m_frameTime * m_speed;

	velocitySkipY = antiMovement(velocitySkipY);

	velocitySkipY = checkMaxSpeed(velocitySkipY, std::abs(velocity.y));

	velocitySkipY = checkMinSpeed(velocitySkipY);

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

 void Player::setFrametime(long double dt)
 {
	 m_frameTime = dt;
 }