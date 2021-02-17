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
	 m_maxSpeed = 90.0f;//100.0f;
	 m_maxSpeedRetardation = 150.0f;//1000.0f;
	 m_baseFlySpeed = 100.0f;
	 m_baseGroundSpeed = 350.0f;
	 m_maxGroundSpeed = 1000.0f;//1400.0f;//900.0f;//700.0f;
	 m_maxFlySpeed = 1000.0f;//1400.0f;//900.0f;//700.0f;//500;
	 m_minSpeed = 0.1f;
	 m_groundSpeed = 0;
	 m_flySpeed = 100.0f;//100.0f;
	 m_dashSpeed = 150.0f;
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
	 m_cooldownDash = 0.0f;
	 m_waitForJump = false;
	 m_jumpWhenLanding = false;
	 m_checkCollideJump = false;
	 m_timer = Timer();
	 m_timer.start();
 }

 Player::~Player()
 {

 }

 void Player::start()
 {
	 m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	 m_playerCamera->setOffset(0, 3.0f, 0);//-0.5);
	 m_rb = m_gameObject->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
	 m_capsuleCollider = m_gameObject->getComponentType<CapsuleCollider>(Component::ComponentEnum::CAPSULE_COLLIDER);

	 m_rb->setGravity(20.0);
 }	

 Vector3 Player::antiMovement(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 constexpr float modifier = 2.4f;
	 constexpr float modifierStop = 9.f;
	 constexpr float modifierFly = 1.1f;//* 1.385f;//1.385f;//1.38f;//1.4f;

	 Vector3 velocityNormal = velocity;
	 velocityNormal.y = 0;
	 float saveY = velocity.y;
	 velocity.y = 0;
	 //When the player stops moving the antiMovement gets bigger
	 float antiMoveSize = m_maxAntiMoveSize;

	 if (moveDirection != Vector3::Zero)
	 {
		 antiMoveSize = m_minAntiMoveSize * modifier;
	 }
	 if (!onGround)
	 {
		 if (moveDirection == Vector3::Zero)
			 antiMoveSize /= modifierStop;
		 else
		 {
			 antiMoveSize = modifierFly;
		 }
	 }

	 if (velocity.Length() > m_minSpeed)
	 {
		 velocityNormal.Normalize();
		 velocity -= velocityNormal* antiMoveSize* velocity.Length()* m_frameTime;
	 }
	 velocity.y = saveY;
	 return velocity;
 }

 Vector3 Player::checkMaxSpeed(Vector3 velocity)
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

 Vector3 Player::checkMinSpeed(const Vector3& velocity)
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
 Vector3 Player::checkDirection(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 constexpr float flyDirectionSize = 125.0f;
	 constexpr float groundDirectionSize = 2500.0f;
	 constexpr float speedChange = 200.0f;

	 float changeDirectionSize = groundDirectionSize;
	 if (!onGround)
		 changeDirectionSize = flyDirectionSize;

	 if (velocity.Dot(moveDirection) < 0.5f && velocity.Length() > m_maxSpeed / 3.0f)
	 {
		 m_flySpeed -= velocity.Length() * speedChange * m_frameTime;
		 m_groundSpeed -= velocity.Length() * speedChange * m_frameTime;
	 }

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
	 constexpr float groundSpeedModifier = 400.0f;
	 constexpr float flySpeedModifier = 400.0f; //200.0f
	 constexpr float groundSpeedNotMove = 1200.0f;//800.0f;
	 constexpr float flySpeedNotMove = 1200.0f;//500.0f;

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
		 m_groundSpeed += groundSpeedModifier * m_frameTime;
		 m_maxFlySpeed += flySpeedModifier * m_frameTime;
	 }
	 else
	 {
		 m_groundSpeed -= m_frameTime * groundSpeedNotMove;
		 m_flySpeed -= m_frameTime * flySpeedNotMove;
	 }

	 if (m_groundSpeed > m_maxGroundSpeed)
		 m_groundSpeed = m_maxGroundSpeed;
	 if (m_flySpeed > m_maxFlySpeed)
		 m_flySpeed = m_maxFlySpeed;
	 if (m_groundSpeed < 0)
		 m_groundSpeed = 0;
	 if (m_flySpeed < 0)
		 m_flySpeed = 0;
 }

 Vector3 Player::dash(Vector3 velocity, Vector3 cameraLook)
 {
	 //Dash
	 if (Input::getInput().keyPressed(Input::Shift) && m_cooldownDash <= 0.0f)
	 {
		 constexpr float minYVelocity = 10.0f;
		 constexpr float maxYVelocity = 50.0f;

		 m_cooldownDash = 2.0f;

		 velocity = { 0, 0, 0 };
		 cameraLook.Normalize();
		 velocity += cameraLook * m_dashSpeed;
		 if (m_ground && velocity.y < minYVelocity)
		 {
			 velocity.y = minYVelocity;
		 }
		 if (velocity.y > maxYVelocity)
		 {
			 velocity.y = maxYVelocity;
		 }
		 m_ground = false;
	 }
	 if (m_cooldownDash > 0.0f)
	 {
		 m_cooldownDash -= 1 * m_frameTime;
	 }

	 return velocity;
 }

 void Player::gravityChange(const Vector3& velocity)
 {
	 constexpr float changeGVelocity = 20.0f;//10.0f)//5.0f)
	 constexpr float bigG = 80.0f;//55);//45);
	 constexpr float smallG = 55.0f;//35);

	 if (velocity.y < changeGVelocity)
		 m_rb->setGravity(bigG);
	 else
		 m_rb->setGravity(smallG);
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

		velocity = jumpPlayer(velocity);

		if (Input::getInput().mouseBeingPressed(Input::LeftButton) && m_ground == true)
		{
			if (m_chargeJump < 50.0f)
			{
				m_chargeJump += 10 * m_frameTime;
			}
		}

	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	m_walljump = false;
	moveDirection.y = 0;
	moveDirection.Normalize();

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;

	velocitySkipY = checkDirection(velocitySkipY, moveDirection, m_ground);

	checkSpeeds(moveDirection);
	velocitySkipY = antiMovement(velocitySkipY, moveDirection, m_ground);
	velocitySkipY += moveDirection * m_frameTime * m_speed;

	velocitySkipY = dash(velocitySkipY, cameraLook);

	velocitySkipY = checkMaxSpeed(velocitySkipY);
	velocitySkipY = checkMinSpeed(velocitySkipY);
	velocitySkipY.y += velocity.y;
	velocity = velocitySkipY;

	gravityChange(velocity);

	m_rb->setVelocity(velocity);

	m_playerCamera->update();

	velocitySkipY.y = 0;
	char msgbuf[1000];
	sprintf_s(msgbuf, "My variable is %f\n", velocitySkipY.Length());
	//OutputDebugStringA(msgbuf);
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	 Vector3 normal = m_capsuleCollider->getCollisionNormal();
	 if (m_waitForJump)
	 {
		 m_waitForJump = false;
	 }
	 if (/*collider->getGameObject()->getName() == "brickCube" && */normal.Dot(Vector3::Up) > 0.8f)
	 {
		 m_ground = true;
		 m_doubleJump = true;
		 m_jetPackFuel = m_jetPackFuelMax;
		 if (m_checkCollideJump)
		 {
			 m_checkCollideJump = false;
			 m_jumpWhenLanding = true;
		 }
	 }
	 if (collider->getGameObject()->getName() == "wall")
	 {
		 m_walljump = true;
	 }
	 if (collider->getGameObject()->getName() == "goal")
	 {
		 m_rb->getTransform()->setPosition(respawn);
		 std::wstring headerMsg = L"Your Time is";
		 m_timer.stop();
		 m_timer.print(headerMsg);
		 m_timer.start();
		
	 }
	 if (!m_ground)
	 {
		 m_addSpeed = true;
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
		 m_timer.stop();
		 m_timer.start();
	 }
 }

 Vector3 Player::jumpPlayer(Vector3 velocity)
 {
	 const float jetpackModifier = 50.0f;
	 if (Input::getInput().keyBeingPressed(Input::Space) && m_ground == false)
	 {
		 if (m_jetPackFuel > 0.0f)
		 {
			 velocity.y += m_jetPackSpeed * m_frameTime;
			 m_jetPackFuel -= jetpackModifier * m_frameTime;
		 }
	 }

	 if (Input::getInput().keyPressed(Input::Space) || m_jumpWhenLanding)
	 {
		 //Checks if the player is in the air and if the playerTrigger has collided with an object
		 if (m_waitForJump && !m_checkCollideJump && !m_ground)
		 {
			 m_checkCollideJump = true;
			 m_waitForJump = false;
		 }
		 else if (m_ground == true)
		 {
			 velocity.y += m_jumpSpeed + m_chargeJump;
			 m_chargeJump = 0.0f;
			 m_ground = false;
			 m_checkCollideJump = false;
			 m_waitForJump = false;
			 m_jumpWhenLanding = false;
		 }
		 else if (m_doubleJump == true)
		 {
			 velocity.y = m_doubleJumpSpeed;
			 m_doubleJump = false;
		 }
		 else if (m_walljump == true)
		 {
			 velocity.y = 30;
		 }
		 m_jumpWhenLanding = false;
	 }

	 if (Input::getInput().keyReleased(Input::Space) && m_doubleJump)
	 {
		 m_jetPackFuel = 0;
	 }

	 return velocity;
 }

 void Player::setRespawn(Vector3 incomingRespawn)
 {
	 respawn = incomingRespawn;
 }

 void Player::setFrametime(long double dt)
 {
	 m_frameTime = dt;
 }

 void Player::setWaitForJump()
 {
	 //Bad solution
	 if (m_rb->getVelocity().y < 0)
		 m_waitForJump = true;
	 else
		 m_waitForJump = false;
 }

 bool Player::getOnGround()
 {
	 return m_ground;
 }