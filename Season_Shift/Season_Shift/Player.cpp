#include "Player.h"

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_yaw = DirectX::XM_2PI/2;
	 m_pitch = 0.0f;
	 m_roll = 0.0f;
	 respawn = { 0, 10, 0 };
	 m_disable = false;
	 m_frameTime = 0.0f;
	 m_speed = 300.0f;
	 m_maxSpeed = 30.0f;
	 m_minSpeed = 0.1f;
	 m_ground = false;
	 m_doubleJump = true;
	 m_jetPackFuel = 50.0f;
	 m_maxAntiMoveSize = 14.3f;
	 m_minAntiMoveSize = 6.0f;
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

 const Vector3& Player::antiMovement(Vector3 velocity, const Vector3& moveDirection)
 {
	 //When the player stops moving the antiMovement gets bigger
	 float antiMoveSize = m_maxAntiMoveSize;
	 if (moveDirection != Vector3::Zero)
	 {
		 const float modifier = 20;
		 antiMoveSize = m_minAntiMoveSize + velocity.Length() * modifier / m_speed;
	 }

	 if (velocity.Length() > m_minSpeed)
	 {
		 Vector3 velocityNormal = velocity;
		 velocityNormal.Normalize();
		 velocity -= velocityNormal * antiMoveSize * velocity.Length() * m_frameTime;
	 }
	 return velocity;
 }

 const Vector3& Player::checkMaxSpeed(Vector3 velocity)
 {
	 if (velocity.Length() > m_maxSpeed)
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

 //Checks if you change direction in movement and sets the previous velocity to zero
 const Vector3& Player::checkDirection(Vector3 velocity, const Vector3& moveDirection)
 {
	 Vector3 check = moveDirection * velocity;
	 if (check.x < 0)
		 velocity.x = 0;
	 if (check.y < 0)
		 velocity.y = 0;
	 if (check.z < 0)
		 velocity.z = 0;
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
		}
		if (Input::getInput().keyBeingPressed(Input::S))
		{
			moveDirection -= cameraForward;
		}
		if (Input::getInput().keyBeingPressed(Input::A))
		{
			moveDirection -= cameraRight;
		}
		if (Input::getInput().keyBeingPressed(Input::D))
		{
			moveDirection += cameraRight;
		}
		if (Input::getInput().keyPressed(Input::Esc))
		{
			exit(0);
		}
		if (Input::getInput().keyPressed(Input::Space))
		{
			if (m_ground == true) 
			{
				velocity += Vector3(0, 50, 0);
				m_ground = false;
			}
			else if(m_doubleJump == true)
			{
				velocity.y = 6;
				m_doubleJump = false;
			}

		}
		if (Input::getInput().keyBeingPressed(Input::Space) && m_ground == false && m_doubleJump == false)
		{
			if (m_jetPackFuel > 0.0f)
			{
				velocity += Vector3(0, 30 * m_frameTime, 0);
				m_jetPackFuel -= 50 * m_frameTime;
			}
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

	if (m_ground == false)
	{
		m_speed = 50.0f;
	}
	else
	{
		m_speed = 300.0f;
	}
	moveDirection.y = 0;
	moveDirection.Normalize();

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;

	velocitySkipY = checkDirection(velocitySkipY, moveDirection);

	velocitySkipY += moveDirection * m_frameTime * m_speed;

	if (m_ground == true)
	{
		velocitySkipY = antiMovement(velocitySkipY, moveDirection);
	}
	velocitySkipY = checkMaxSpeed(velocitySkipY);

	velocitySkipY = checkMinSpeed(velocitySkipY);

	velocitySkipY.y = velocity.y;
	velocity = velocitySkipY;

	m_rb->setVelocity(velocity);

	m_playerCamera->update();

	/*char msgbuf[1000];
	sprintf_s(msgbuf, "My variable is %f, %f, %f\n", check.x, check.y, check.z);
	OutputDebugStringA(msgbuf);*/
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	 if (collider->getGameObject()->getName() == "brickCube") 
	 {
		 m_ground = true;
		 m_doubleJump = true;
		 m_jetPackFuel = 50.0f;
	 }
	
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