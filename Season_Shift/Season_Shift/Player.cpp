#include "Player.h"
#include "Logger.h"
#include <imgui_impl_win32.h>

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_yaw = DirectX::XM_2PI/2;
	 m_pitch = 0.0f;
	 m_roll = 0.0f;
	 respawn = { 0, 10, 0 };
	 m_normal = { 0, 0, 0 };
	 m_disable = false;
	 m_frameTime = 0.0f;
	 m_speed = 300.0f;
	 m_maxSpeed = 90.0f;
	 m_maxSpeedRetardation = 150.0f;
	 m_baseFlySpeed = 100.0f;
	 m_baseGroundSpeed = 350.0f;
	 m_maxGroundSpeed = 1000.0f;
	 m_maxFlySpeed = 1000.0f;
	 m_minSpeed = 0.1f;
	 m_groundSpeed = 0;
	 m_flySpeed = 100.0f;
	 m_dashSpeed = 150.0f;
	 m_ground = false;
	 m_doubleJump = true;
	 m_jetPackFuelMax = 10.0f;
	 m_jetPackFuel = m_jetPackFuelMax;
	 m_jetPackSpeed = 67.0f;
	 m_maxAntiMoveSize = 14.3f;
	 m_minAntiMoveSize = 6.0f;
	 m_jumpSpeed = 26.0f;
	 m_doubleJumpSpeed = 30.0f;
	 m_cooldownDash = 0.0f;
	 m_waitForJump = false;
	 m_jumpWhenLanding = false;
	 m_checkCollideJump = false;
	 m_walljump = false;
	 m_fly = false;
	 m_timer = Timer();
	 m_timer.start();
	 m_oldFrameTime = 0.0f;

	 m_oldMoveDirection = Vector3::Zero;
	 m_lerp = 0.96f;
 }

 Player::~Player()
 {

 }

 void Player::start()
 {
	 m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	 m_playerCamera->setOffset(0, 3.0f, 0);
	 m_rb = m_gameObject->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
	 m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
	 m_capsuleCollider = m_gameObject->getComponentType<CapsuleCollider>(Component::ComponentEnum::CAPSULE_COLLIDER);

	 m_rb->setGravity(55.0);
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
		if (Input::getInput().keyPressed(Input::F))
		{
			m_fly = !m_fly;
		}

		velocity = jumpPlayer(velocity);

	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	moveDirection.y = 0;
	moveDirection.Normalize();

	m_oldMoveDirection = Vector3::Lerp(m_oldMoveDirection, moveDirection, m_frameTime * m_lerp);

	velocity = playerFly(velocity);

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;

	velocitySkipY = checkDirection(velocitySkipY, moveDirection, m_ground);

	checkSpeeds(moveDirection);
	velocitySkipY = antiMovement(velocitySkipY, moveDirection, m_ground);
	velocitySkipY += moveDirection * m_frameTime * m_speed;

	velocitySkipY = dash(velocitySkipY, cameraLook);
	velocitySkipY = slowPlayer(velocitySkipY);

	velocitySkipY = checkMaxSpeed(velocitySkipY);
	velocitySkipY = checkMinSpeed(velocitySkipY);
	velocitySkipY.y += velocity.y;
	velocity = velocitySkipY;

	gravityChange(velocity);
	wallRunning(velocity);
	m_rb->setVelocity(velocity);

	m_playerCamera->update();

	velocitySkipY.y = 0;

	//char msgbuf[1000];
	//sprintf_s(msgbuf, "My variable is %f\n", velocitySkipY.Length());
	//OutputDebugStringA(msgbuf);

	ImGui::Begin("Player Info");
	{
		ImGui::Text("Velocity: %f, %f, %f", velocity.x, velocity.y, velocity.z);
		ImGui::Text("Speed: %f", velocity.Length());
		ImGui::Text("Speed (XZ): %f", velocitySkipY.Length());
		ImGui::Text("Dash cooldown: %f", m_cooldownDash);
		//ImGui::SliderFloat("Lerp", &m_lerp, 0.0, 10.0);
	}
	ImGui::End();
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	 m_normal = m_capsuleCollider->getCollisionNormal();
	 if (m_waitForJump)
	 {
		 m_waitForJump = false;
	 }
	 if (/*collider->getGameObject()->getName() == "brickCube" && */m_normal.Dot(Vector3::Up) > 0.8f)
	 {
		 m_ground = true;
		 m_walljump = false;
		 m_doubleJump = true;
		 m_jetPackFuel = m_jetPackFuelMax;
		 m_roll = 0.0f;
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
		 std::wstring msg = L"Your Time was";
		 getTime(msg);
		
	 }

 }

 Vector3 Player::antiMovement(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 constexpr float modifierRun = 2.4f;
	 constexpr float modifierFlyStop = 1.f / 5.f;//1.f/9.f;
	 constexpr float modifierFly = 1.1f;

	 Vector3 velocityNormal = velocity;
	 velocityNormal.y = 0;
	 float saveY = velocity.y;
	 velocity.y = 0;

	 //Not true anymore 
	 //When the player stops moving the antiMovement gets bigger
	 float antiMoveSize = m_maxAntiMoveSize;

	 if (moveDirection != Vector3::Zero)
	 {
		 antiMoveSize = m_minAntiMoveSize * modifierRun;
	 }
	 if (!onGround)
	 {
		 if (moveDirection == Vector3::Zero)
			 antiMoveSize = modifierFlyStop;
		 else
		 {
			 antiMoveSize = modifierFly;
		 }
	 }

	 if (velocity.Length() > m_minSpeed)
	 {
		 velocityNormal.Normalize();
		 velocity -= velocityNormal * antiMoveSize * velocity.Length() * m_frameTime;
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

 //Checks if you change direction in movement and changes the speed or velocity
 Vector3 Player::checkDirection(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 constexpr float flyDirectionSize = -17.5f;
	 constexpr float groundDirectionSize = 2500.0f;
	 constexpr float speedChange = 100.0f;

	 float changeDirectionSize = groundDirectionSize;
	 if (!onGround)
		 changeDirectionSize = flyDirectionSize;

	 if (m_oldMoveDirection.Dot(moveDirection) < 0.0f && velocity.Length() > m_maxSpeed / 3.0f)
	 {
		 m_flySpeed -= speedChange * velocity.Length() * m_frameTime;
		 m_groundSpeed -= speedChange * velocity.Length() * m_frameTime;
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
	 constexpr float flySpeedModifier = 400.0f;
	 constexpr float groundSpeedNotMove = 1200.0f;
	 constexpr float flySpeedNotMove = 1200.0f;

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
	 constexpr float minYVelocity = 10.0f;
	 constexpr float maxYVelocity = 50.0f;

	 //Dash
	 if (Input::getInput().keyPressed(Input::Shift) && m_cooldownDash <= 0.0f)
	 {
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
	 if (m_walljump == true)
		 m_rb->setGravity(30.0);
	 else if (velocity.y < changeGVelocity)
		 m_rb->setGravity(bigG);
	 else
		 m_rb->setGravity(smallG);
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
		 m_roll = 0.0f;
		 m_pitch = 0.0f;
		 m_yaw = 0.0f;
		 std::wstring msg = L"Your survived for";
		 getTime(msg);
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
		 if (m_walljump == true)
		 {
			 velocity.x += m_jumpSpeed * m_roll * -1 * 9;
			 velocity.y += 25;
			 m_walljump = false;
		 }
		 else if (m_waitForJump && !m_checkCollideJump && !m_ground)
		 {
			 m_checkCollideJump = true;
			 m_waitForJump = false;
		 }
		 else if (m_ground == true)
		 {
			 velocity.y += m_jumpSpeed;
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
		 m_jumpWhenLanding = false;
	 }

	 if (Input::getInput().keyReleased(Input::Space) && m_doubleJump)
	 {
		 m_jetPackFuel = 0;
	 }

	 return velocity;
 }

 Vector3 Player::slowPlayer(Vector3 velocity)
 {
	 if (Input::getInput().mouseBeingPressed(Input::MouseKeys::LeftButton))
	 {
		 Vector3 normal = velocity;
		 normal.Normalize();
		 velocity -= 10.0f * m_frameTime * normal;
	 }
	 return velocity;
 }

 void Player::setRespawn(Vector3 incomingRespawn)
 {
	 respawn = incomingRespawn;
 }

 void Player::setFrametime(long double dt)
 {
	 m_oldFrameTime = m_frameTime;
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

 void Player::getTime(std::wstring msg) 
 {
	 m_timer.stop();
	 m_timer.print(msg);
	 m_timer.start();
 }

 void Player::wallRunning(DirectX::SimpleMath::Vector3 velocity) {
	 //Clown Code need fixing afap
	 if (m_walljump == false) {
		 if (m_roll > 0)
		 {
			 m_roll -= 0.3 * m_frameTime * 2.5;
		 }
		 else if (m_roll < 0)
		 {
			 m_roll += 0.3 * m_frameTime * 2.5;
		 }
		 else
		 {
			 m_roll = 0.0;
		 }
	 }
	 else
	 {
		 if (m_roll > -0.3 && m_normal.x > 0)
		 {
			 m_roll += -1 * m_normal.x * DirectX::XM_PI / 7 * m_frameTime * 5;
		 }
		 else if (m_roll < 0.3 && m_normal.x < 0)
		 {
			 m_roll += -1 * m_normal.x * DirectX::XM_PI / 7 * m_frameTime * 5;
		 }

	 }
	 //////////////////////////////////////////
 }
 //Debug feature
 Vector3 Player::playerFly(Vector3 velocity)
 {
	 if (m_fly)
	 {
		 velocity = Vector3::Zero;
		 Vector3 position = m_transform->getPosition();
		 Vector3 cameraForward = m_playerCamera->getForward();
		 Vector3 cameraRight = m_playerCamera->getRight();
		 if (Input::getInput().keyBeingPressed(Input::W))
		 {
			 position += 100.0f * m_frameTime * cameraForward;
		 }
		 if (Input::getInput().keyBeingPressed(Input::A))
		 {
			 position -= 100.0f * m_frameTime * cameraRight;
		 }
		 if (Input::getInput().keyBeingPressed(Input::S))
		 {
			 position -= 100.0f * m_frameTime * cameraForward;
		 }
		 if (Input::getInput().keyBeingPressed(Input::D))
		 {
			 position += 100.0f * m_frameTime * cameraRight;
		 }
		 if (Input::getInput().keyBeingPressed(Input::Space))
		 {
			 position.y += 100.0f * m_frameTime;
		 }
		 if (Input::getInput().keyBeingPressed(Input::Shift))
		 {
			 position.y -= 100.0f * m_frameTime;
		 }
		 m_transform->setPosition(position);
	 }
	 return velocity;
 }