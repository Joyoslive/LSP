#include "Player.h"
#include "Logger.h"

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
	 m_maxSpeed = 100.0f;
	 m_maxSpeedRetardation = 1000.0f;
	 //m_oldMaxSpeed = 700.0f;
	 m_baseFlySpeed = 100.0f;
	 m_baseGroundSpeed = 350.0f;
	 m_maxGroundSpeed = 700.0f;
	 m_maxFlySpeed = 700.0f;//500;
	 m_minSpeed = 0.1f;
	 //m_groundSpeed = 350.0f;//300.0f;
	 m_groundSpeed = 0;
	 m_flySpeed = 100.0f;//100.0f;
	 m_ground = false;
	 m_doubleJump = true;
	 m_jetPackFuelMax = 10.0f;
	 m_jetPackFuel = m_jetPackFuelMax;
	 m_jetPackSpeed = 67.0f;//60.0f;//30.0f;
	 m_maxAntiMoveSize = 14.3f;
	 m_minAntiMoveSize = 6.0f;
	 m_chargeJump = 0.0f;
	 m_jumpSpeed = 26.0f;//15.0f;
	 m_doubleJumpSpeed = 30.0f;//9.0f;
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

	 m_rb->setGravity(20.0);
 }	

 const Vector3& Player::antiMovement(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 Vector3 velocityNormal = velocity;
	 Vector3 fakeVelocity = velocity;
	 if (velocityNormal.y < 0)
	 {
		 velocityNormal.y = 0;
		 fakeVelocity.y = 0;
	 }

	 //When the player stops moving the antiMovement gets bigger
	 float antiMoveSize = m_maxAntiMoveSize;

	 if (moveDirection != Vector3::Zero)
	 {
		 const float modifier = 20.0f / 300.0f;
		 antiMoveSize = m_minAntiMoveSize + fakeVelocity.Length() * modifier;
		 //antiMoveSize = 0;
	 }
	 if (!onGround)
	 {
		 if (moveDirection == Vector3::Zero)
			 antiMoveSize /= 9.f;
		 else
		 {
			 antiMoveSize = 1.38f;//1.4f;
		 }
	 }

	 if (velocity.Length() > m_minSpeed)
	 {
		 velocityNormal.Normalize();
		 velocity -= velocityNormal * antiMoveSize * fakeVelocity.Length() * m_frameTime;
	 }
	 return velocity;
 }

 const Vector3& Player::checkMaxSpeed(Vector3 velocity)
 {
	 Vector3 velocitySkipY = velocity;
	 velocitySkipY.y = 0;
	 if (velocitySkipY.Length() > m_maxSpeed)
	 {
		 Vector3 velocityNormal = velocity;
		 velocityNormal.Normalize();
		 velocity -= velocityNormal * m_maxSpeedRetardation * m_frameTime;
	 }
	 return velocity;
 }

 const Vector3& Player::checkMinSpeed(const Vector3& velocity)
 {
	 if (velocity.Length() < m_minSpeed)
		 return Vector3::Zero;
	 return velocity;
 }

 int signOf(const float& value)
 {
	 if (value < 0)
		 return -1;
	 else
		 return 1;
 }

 //Checks if you change direction in movement and sets the previous velocity to zero
 const Vector3& Player::checkDirection(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 float changeDirectionSize = 2500.0f;
	 if (!onGround)
		 changeDirectionSize = 125.0f;//100.0f;//1000.0f;//125.0f;

	 Vector3 check = moveDirection * velocity;
	 if (check.x < 0)
		 velocity.x -= m_frameTime * changeDirectionSize * signOf(velocity.x) * std::abs(moveDirection.x);
	 if (check.y < 0)
		 velocity.y -= m_frameTime * changeDirectionSize * signOf(velocity.y) * std::abs(moveDirection.y);
	 if (check.z < 0)
		 velocity.z -= m_frameTime * changeDirectionSize * signOf(velocity.z) * std::abs(moveDirection.z);
	 return velocity;
 }

 void Player::checkSpeeds(const Vector3& moveDirection)
 {
	 if (m_ground == false)
	 {
		 m_speed = m_baseFlySpeed + m_flySpeed;
	 }
	 else
	 {
		 m_speed = m_baseGroundSpeed + m_groundSpeed;
	 }

	 if (moveDirection != Vector3::Zero)
	 {
		 m_groundSpeed += 400 * m_frameTime;
		 m_maxFlySpeed += 200 * m_frameTime;
		 if (m_groundSpeed > m_maxGroundSpeed)
			 m_groundSpeed = m_maxGroundSpeed;
		 if (m_flySpeed > m_maxFlySpeed)
			 m_flySpeed = m_maxFlySpeed;
	 }
	 else
	 {
		 m_groundSpeed -= m_frameTime * 800;
		 m_flySpeed -= m_frameTime * 500;
		 if (m_groundSpeed < 0)
			 m_groundSpeed = 0;
		 if (m_flySpeed < 0)
			 m_flySpeed = 0;
	 }
 }

 void Player::update()
 {
	 if (m_disable == false)
	 {
		 lookAround();
	 }

	detectDeath(-35.0f);
	Vector3 velocity = m_rb->getVelocity();
	Vector3 cameraForward = m_playerCamera->getForward();
	Vector3 cameraRight = m_playerCamera->getRight();
	Vector3 cameraLook = m_playerCamera->getLookDirection();
	
	Vector3 moveDirection = Vector3::Zero;
	
	if (Input::getInput().keyPressed(Input::X))
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

		if (Input::getInput().keyBeingPressed(Input::Space) && m_ground == false)
		{
			if (m_jetPackFuel > 0.0f)
			{
				velocity.y += m_jetPackSpeed * m_frameTime;
				m_jetPackFuel -= 50 * m_frameTime;
			}
		}

		if (Input::getInput().keyPressed(Input::Space))
		{
			if (m_ground == true) 
			{
				velocity.y += m_jumpSpeed + m_chargeJump;
				m_chargeJump = 0.0f;
				m_ground = false;
			}
			else if(m_doubleJump == true)
			{
				velocity.y = m_doubleJumpSpeed;
				m_doubleJump = false;
			}

		}
		if (Input::getInput().mouseBeingPressed(Input::LeftButton) && m_ground == true)
		{
			if (m_chargeJump < 50.0f)
			{
				m_chargeJump += 10 * m_frameTime;
			}
		}
		if (Input::getInput().keyReleased(Input::Space) && m_doubleJump)
		{
			m_jetPackFuel = 0;
		}

	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}

	moveDirection.y = 0;
	moveDirection.Normalize();

	checkSpeeds(moveDirection);

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;

	velocitySkipY = checkDirection(velocitySkipY, moveDirection, m_ground);

	velocitySkipY += moveDirection * m_frameTime * m_speed;

	//Dash
	if (Input::getInput().keyPressed(Input::Shift))
	{
		velocitySkipY = { 0, 0, 0 };
		cameraLook.Normalize();
		velocitySkipY += cameraLook * 600.0f;
		if (m_ground && velocitySkipY.y < 10.0f)
		{
			velocitySkipY.y = 10.0f;
		}
		if (velocitySkipY.y > 50.0f)
		{
			velocitySkipY.y = 50.0f;
		}
		m_ground = false;
	}

	/*if (m_addSpeed)
	{
		Vector3 velocityNormalize = velocitySkipY;
		velocityNormalize.y = 0;
		velocityNormalize.Normalize();
		//velocitySkipY += 1600 * velocityNormalize;
		m_addSpeed = false;
	}*/

	velocitySkipY = antiMovement(velocitySkipY, moveDirection, m_ground);
	velocitySkipY = checkMaxSpeed(velocitySkipY);
	velocitySkipY = checkMinSpeed(velocitySkipY);
	velocitySkipY.y += velocity.y;
	velocity = velocitySkipY;

	if (velocity.y < 20.0f)//10.0f)//5.0f)
		m_rb->setGravity(80.0f);//55);//45);
	else
		m_rb->setGravity(55.0f);//35);

	m_rb->setVelocity(velocity);

	m_playerCamera->update();

	velocitySkipY.y = 0;
	char msgbuf[1000];
	sprintf_s(msgbuf, "My variable is %f\n", velocitySkipY.Length());
	OutputDebugStringA(msgbuf);
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	 if (collider->getGameObject()->getName() == "brickCube") 
	 {
		 m_ground = true;
		 m_doubleJump = true;
		 m_jetPackFuel = m_jetPackFuelMax;
	 }
	 if (!m_ground)
	 {
		 m_addSpeed = true;
	 }

	 m_ground = true;

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