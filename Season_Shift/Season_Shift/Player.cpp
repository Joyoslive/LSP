#include "pch.h"
#include "Player.h"
#include "Logger.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "OrientedBoxCollider.h"
#include "CapsuleCollider.h"
#include "Transform.h"
#include "Move.h"
#include "Bounce.h"
#include <imgui_impl_win32.h>
#include "Graphics/Graphics.h"
#include "Graphics/2D/ISprite.h"
#include "ParticleSystemComponent.h"

using namespace DirectX::SimpleMath;

 Player::Player()
 {
	 m_respawn = { 0, 10, 0 };
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
	 m_goalTimer = Timer();
	 m_timer.start();
	 m_goalTimer.start();
	 m_oldFrameTime = 0.0f;
	 m_wallTimer = 0.0f;
	 m_oldCollider = NULL;
	 m_oldMoveDirection = Vector3::Zero;
	 m_hooked = false;
	 m_movObj = false;
	 m_hookDist = 0;
	 m_hookPoint = Vector3(0, 0, 0);
	 m_deltaPos = Vector3(0, 0, 0);
	 m_velocityY = 0;
	 m_movPos = 0;
	 m_movAlt = 1033.33;
	 m_movSpeed = { 0, 0, 0 };
	 m_trampoline = false;
	 m_trampolineAngle = { 0, 0, 0 };
	 m_maxYSpeed = 100.0f;
	 m_sLR = m_sLS = m_sLT = 0;
	 m_landingPartEmittId = -1;
 }

 Player::~Player()
 {
	
 }

 int signOf(const float& value)
 {
	 if (value < 0)
		 return -1;
	 else
		 return 1;
 }

 void Player::start()
 {
	 m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	 m_playerCamera->setOffset(0, 2.0f, 0);
	 m_rb = m_gameObject->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
	 m_playerCamera->setRotation(0,0,0);
	 m_capsuleCollider = m_gameObject->getComponentType<CapsuleCollider>(Component::ComponentEnum::CAPSULE_COLLIDER);
	 m_logicPlayerCamera = std::make_shared<PlayerCameraMovement>(); 
	 m_gameObject->AddComponent(m_logicPlayerCamera);
	 m_logicPlayerCamera->start();

	 m_playerPartSys = std::dynamic_pointer_cast<ParticleSystemComponent>(m_gameObject->AddComponent(std::make_shared<ParticleSystemComponent>(1000, 1)));
	 m_landingPartEmittId = m_playerPartSys->addEmitter(1000, 0, 0, Vector3(1, 1, 0), Vector3::Zero, Vector3(0, 0, 7));
	 //	80 / 2 = 40
	
	 m_rb->setGravity(55.0);
 }	

 void Player::update()
 {

	 if (m_createOnce)
	 {
		 m_velocitySprite = m_gameObject->getScene()->getGraphics()->getResourceDevice()->createSprite("Hello", L"Textures/Sprites/Fonts/font.spritefont", 275, 675);
		 m_gameObject->getScene()->getGraphics()->addToSpriteBatch(m_velocitySprite);
		 m_createOnce = false;
	 }

	detectDeath(-35.0f);
	Vector3 velocity = m_rb->getVelocity();
	Vector3 cameraForward = m_playerCamera->getForward();
	Vector3 cameraRight = m_playerCamera->getRight();
	Vector3 cameraLook = m_playerCamera->getLookDirection();
	
	Vector3 moveDirection = Vector3::Zero;
	Vector3 moveDirection2 = Vector3::Zero;
	//Vector3 moveSpeed = Vector3::Zero;


	if (m_hooked)
	{
		LineVariables settings;
		settings.startPos = m_transform->getPosition();
		settings.endPos = m_hookPoint;
		settings.color = Vector3::Zero;
		settings.offset = Vector3(1.0, 0.4, 0.0);
		settings.thickness = Vector2(0.1, 0.1);

		m_gameObject->getScene()->getGraphics()->renderLine(settings);
	}

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
		//Checks for wallRunning direction the player should Move
		Vector3 up = Vector3::Up;
		Vector3 cross = up.Cross(m_normal);
		float dot = cross.Dot(cameraForward);
		if (dot < 0)
			cross *= -1;

		if (Input::getInput().keyBeingPressed(Input::W))
		{
			if (m_walljump)
				moveDirection += cross;
			else
				moveDirection += cameraForward;
		}
		if (Input::getInput().keyBeingPressed(Input::S))
		{
			if (m_walljump)
				moveDirection -= cross;
			else
				moveDirection -= cameraForward;
		}
		if (Input::getInput().keyBeingPressed(Input::A))
		{
			if (!m_walljump)
				moveDirection -= cameraRight;
		}
		if (Input::getInput().keyBeingPressed(Input::D))
		{
			if (!m_walljump)
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
		
		grappleHook(cameraLook);

		velocity = jumpPlayer(velocity);

	}
	if (Input::getInput().keyPressed(Input::L))
	{
		Input::getInput().lockMouse();
	}
	if(m_wallTimer <= 0)
	{
		m_oldCollider = NULL;
	}
	
	moveDirection2 = moveObjectCheck(moveDirection2);

	moveDirection.y = 0;
	moveDirection.Normalize();

	constexpr float lerpMoveDirection = 0.96f;
	m_oldMoveDirection = Vector3::Lerp(m_oldMoveDirection, moveDirection, m_frameTime * lerpMoveDirection);

	velocity = playerFly(velocity);

	Vector3 velocitySkipY = velocity;
	velocitySkipY.y = 0;

	m_logicPlayerCamera->runShake(moveDirection, m_ground, m_walljump, velocitySkipY.Length(), m_maxSpeed);

	velocitySkipY = checkDirection(velocitySkipY, moveDirection, m_ground);

	checkSpeeds(moveDirection);
	velocitySkipY = antiMovement(velocitySkipY, moveDirection, m_ground);

	if (m_movObj == true)
	{
		Vector3 moveSpeed = m_movSpeed;
		moveSpeed.y = 0;

		if (moveDirection == Vector3::Zero || (moveDirection.Dot(moveSpeed) > 0.9f && velocitySkipY.Length() < moveSpeed.Length()))
		{
			velocitySkipY = moveSpeed;
		}

	}
	if (m_trampoline == true)
	{
		velocitySkipY += m_trampolineAngle * 100;
	}
	else
	{
		velocitySkipY += moveDirection * m_frameTime * m_speed;
	}

	velocitySkipY = dash(velocitySkipY, cameraLook);
	velocitySkipY = slowPlayer(velocitySkipY);

	velocitySkipY = checkMaxSpeed(velocitySkipY);
	velocitySkipY = checkMinSpeed(velocitySkipY);
	velocitySkipY.y += velocity.y;
	m_velocityY = moveDirection2.y * 14.4;
	if (m_velocityY < 0) {
		Vector3 hold = m_transform->getPosition();
		hold -= m_deltaPos;
		m_transform->setPosition(hold);
	}
		
	velocity = velocitySkipY;
	velocity = checkYMaxSpeed(velocity);

	gravityChange(velocity);
	wallRunning(velocity);
	m_rb->setVelocity(velocity);

	m_oldVelocity = velocity;

	m_playerCamera->update();

	velocitySkipY.y = 0;

	speedLines(velocitySkipY, velocity.y);
	m_logicPlayerCamera->changeFOV(velocity, m_maxSpeed, m_maxYSpeed);

	//char msgbuf[1000];
	//sprintf_s(msgbuf, "My variable is %f\n", velocity.y / m_maxYSpeed);
	//OutputDebugStringA(msgbuf);
	m_trampoline = false;
	ImGui::Begin("Player Info");
	{
		ImGui::Text("Velocity: %f, %f, %f", velocity.x, velocity.y, velocity.z);
		ImGui::Text("Speed: %f", velocity.Length());
		ImGui::Text("Speed (XZ): %f", velocitySkipY.Length());
		ImGui::Text("Dash cooldown: %f", m_cooldownDash);
		ImGui::Text("Normal:%f, %f, %f", m_normal.x, m_normal.y, m_normal.z);
		ImGui::SliderFloat("Speed", &m_movAlt, 1000.0, 1050.0);
		ImGui::Text("On Ground %d", m_ground);
		//ImGui::Text("Roll: %f", m_roll);
		//ImGui::SliderFloat("Lerp", &m_lerp, 0.0, 10.0);
	}
	ImGui::End();

	std::string text = "Velocity: " + std::to_string(velocity.Length()) + "\n";
	m_velocitySprite->setText(text);
 }

 void Player::onCollision(Ref<Collider> collider)
 {
	 constexpr float floorCheck = 0.8f;
	 constexpr float wallCheck = 0.8f;

	 m_normal = m_capsuleCollider->getCollisionNormal();
	 if (m_waitForJump)
	 {
		 m_waitForJump = false;
	 }
	 if (m_normal.Dot(Vector3::Up) > floorCheck && !m_hooked)
	 {
		 m_movObj = false;
		 m_oldCollider = NULL;
		 m_ground = true;
		 m_walljump = false;
		 m_doubleJump = true;
		 m_jetPackFuel = m_jetPackFuelMax;
		 if (m_checkCollideJump)
		 {
			 m_checkCollideJump = false;
			 m_jumpWhenLanding = true;
		 }
	 }
	 else if (fabs(m_normal.Dot(m_playerCamera->getRight())) > wallCheck && m_oldCollider != collider && collider->getGameObject()->getName() != "checkpoint")
	 {
		 m_movObj = false;
		 m_walljump = true;
		 m_oldCollider = collider;
	 }
	 if (m_logicPlayerCamera->shake(m_oldVelocity, m_normal))
	 {
		 m_playerPartSys->reviveEmitter(m_landingPartEmittId, 0.5f);
	 }

	 if (collider->getGameObject()->getName() == "goal")
	 {
		 detectDeath(FLT_MAX);
		 m_respawn = { 0, 10, 0 };
		 m_rb->getTransform()->setPosition(m_respawn);
		 /*std::wstring msg = L"Your Time was";
		 getTime(msg);*/
		
	 }
	 if (collider->getGameObject()->getName() == "checkpoint")
	 {
		 m_respawn = collider->getGameObject()->getTransform()->getPosition();
		 m_respawn.x += 2;
	 }
	 if (collider->getGameObject()->getName() == "moving")
	 {
		 m_movObj = true;
		 m_deltaPos = collider->getGameObject()->getTransform()->getDeltaPosition();
		 m_movSpeed = collider->getGameObject()->getComponentType<Move>(Component::ComponentEnum::LOGIC)->getSpeed();

	 }
	 if (collider->getGameObject()->getName() == "trampoline")
	 {
		 m_trampoline = true;
		 m_trampolineAngle = collider->getGameObject()->getComponentType<Bounce>(Component::ComponentEnum::LOGIC)->getAngle();

	 }
 }

 Vector3 Player::antiMovement(Vector3 velocity, const Vector3& moveDirection, const bool& onGround)
 {
	 constexpr float modifierRun = 2.4f;
	 constexpr float modifierFlyStop = 1.f / 5.f;
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

 Vector3 Player::checkYMaxSpeed(Vector3 velocity)
 {
	 if (fabs(velocity.y) > m_maxYSpeed)
		 velocity.y = signOf(velocity.y) * m_maxYSpeed;
	 return velocity;
 }

 Vector3 Player::checkMinSpeed(const Vector3& velocity)
 {
	 if (velocity.Length() < m_minSpeed)
		 return Vector3::Zero;
	 return velocity;
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
	 constexpr float maxYVelocity = 30.0f;

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
		 m_cooldownDash -= m_frameTime;
	 }
	 return velocity;
 }

 void Player::gravityChange(const Vector3& velocity)
 {
	 constexpr float changeGVelocity = 25.9f;
	 constexpr float bigG = 95.0f;
	 constexpr float smallG = 55.0f;
	 constexpr float wallJumpG = 60.0f;

	 if (m_walljump == true)
		 m_rb->setGravity(wallJumpG);
	 else if (velocity.y < changeGVelocity)
		 m_rb->setGravity(bigG);
	 else
		 m_rb->setGravity(smallG);

	 if (m_hooked)
		 m_rb->setGravity(bigG);

	 if (m_fly)
		 m_rb->setGravity(0.0f);
 }

 void Player::detectDeath(float death) 
 {
	 if (m_rb->getTransform()->getPosition().y < death)
	 {
		 m_rb->getTransform()->setPosition(m_respawn);
		 m_logicPlayerCamera->resetCamera();
		 std::wstring msg = L"Your survived for";
		 getTime(msg);
		 //goalTimerGetTime();
		 m_rb->setVelocity(Vector3::Zero);
		 m_flySpeed = 0;
		 m_groundSpeed = 0;
		 m_hooked = false;
		 m_rb->stopPendelMotion();
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
		 constexpr float wallTime = 0.04f;
		 constexpr float wallJumpSpeed = 25.0f;
		 constexpr float wallPushSpeed = 60.0f;

		 if (m_walljump == true)
		 {
			 m_wallTimer = wallTime;

			 Vector3 cameraRight = m_playerCamera->getRight();
			 cameraRight.x = fabs(cameraRight.x);
			 cameraRight.y = fabs(cameraRight.y);
			 cameraRight.z = fabs(cameraRight.z);
			 Vector3 normal = cameraRight * m_normal;

			 normal.Normalize();

			 velocity += normal * wallPushSpeed;
			 velocity.y = wallJumpSpeed;
			 m_walljump = false;

			 m_doubleJump = true;
			 m_jetPackFuel = m_jetPackFuelMax;
			 m_ground = false;
		 }
		 //Checks if the player is in the air and if the playerTrigger has collided with an object
		 else if (m_waitForJump && !m_checkCollideJump && !m_ground)
		 {
			 m_checkCollideJump = true;
			 m_waitForJump = false;
		 }
		 else if (m_ground == true)
		 {
			 if (m_movObj == true)
				 velocity.y = m_jumpSpeed + m_velocityY*6;
			 else
 				velocity.y = m_jumpSpeed;
			 m_ground = false;
			 m_checkCollideJump = false;
			 m_waitForJump = false;
			 m_jumpWhenLanding = false;
			 m_movObj = false;
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
	 constexpr float slowModifier = 50.0f;
	 if (Input::getInput().mouseBeingPressed(Input::MouseKeys::RightButton))
	 {
		 Vector3 normalVelocity = velocity;
		 normalVelocity.Normalize();
		 velocity -= slowModifier * m_frameTime * normalVelocity;
	 }
	 return velocity;
 }

 void Player::setRespawn(const Vector3& incomingRespawn)
 {
	 m_respawn = incomingRespawn;
 }

 void Player::setFrametime(long double dt)
 {
	 m_oldFrameTime = m_frameTime;
	 m_frameTime = dt;
	 m_logicPlayerCamera->updateFrameTime(m_frameTime);
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

 long double Player::goalTimerGetTime()
 {
	 m_goalTimer.stop();
	 m_goalTimer.start();
	 return m_goalTimer.getTime(Timer::Duration::SECONDS);
 }

 void Player::wallRunning(const Vector3& velocity) 
 {
	 if (m_wallTimer > 0.0f)
	 {
		 m_wallTimer -= m_frameTime;
	 }
	 
	 if (m_walljump == false) 
	 {
		 m_logicPlayerCamera->wallRunning(m_walljump, Vector3(0,0,0));
	 }
	 else //if (fabs(m_normal.Dot(m_playerCamera->getRight())) > 0.8f)
	 {
		 //cast ray
		 constexpr float maxDist = 1.25f;
		 std::vector<Ref<GameObject>> scene = getGameObject()->getScene()->getSceneGameObjects();
		 float dist = FLT_MAX;
		 bool noHit = true;
		 if (m_normal.Length() != 0)
		 {
			 for (auto& go : scene)
			 {
				 Ref<OrientedBoxCollider> obb = go->getComponentType<OrientedBoxCollider>(Component::ComponentEnum::ORIENTED_BOX_COLLIDER);
				 if (obb != nullptr)
				 {
					 float d = 10000000000000;
					 if (obb->getInternalCollider().Intersects(m_playerCamera->getCamera()->getPosition(), -m_normal, d))
					 {
						 if (d < dist) dist = d;
						 noHit = false;
					 }
				 }
			 }
		 }
		 if (dist > maxDist || noHit)
		 {
			 m_walljump = false;
			 return;
		 }

		 Vector3 cameraRight = m_playerCamera->getRight();
		 Vector3 normal = cameraRight * m_normal;
		 normal.Normalize();
		 bool wallRunning = m_walljump;
		 if (fabs(m_normal.Dot(m_playerCamera->getRight())) <= 0.8f)
			 wallRunning = false;
		 m_logicPlayerCamera->wallRunning(wallRunning, normal);
	 }
	 /*else
	 {
		 m_walljump = false;
		 m_oldCollider = nullptr;
	 }*/
 }

 //Debug feature
 Vector3 Player::playerFly(Vector3 velocity)
 {
	 constexpr float flySpeed = 500.0f;

	 if (m_fly)
	 {
		 velocity = Vector3::Zero;
		 Vector3 position = m_transform->getPosition();
		 Vector3 cameraForward = m_playerCamera->getForward();
		 Vector3 cameraRight = m_playerCamera->getRight();
		 if (Input::getInput().keyBeingPressed(Input::W))
		 {
			 position += flySpeed * m_frameTime * cameraForward;
		 }
		 if (Input::getInput().keyBeingPressed(Input::A))
		 {
			 position -= flySpeed * m_frameTime * cameraRight;
		 }
		 if (Input::getInput().keyBeingPressed(Input::S))
		 {
			 position -= flySpeed * m_frameTime * cameraForward;
		 }
		 if (Input::getInput().keyBeingPressed(Input::D))
		 {
			 position += flySpeed * m_frameTime * cameraRight;
		 }
		 if (Input::getInput().keyBeingPressed(Input::Space))
		 {
			 position.y += flySpeed * m_frameTime;
		 }
		 if (Input::getInput().keyBeingPressed(Input::Shift))
		 {
			 position.y -= flySpeed * m_frameTime;
		 }
		 m_transform->setPosition(position);
	 }
	 return velocity;
 }

 void Player::clearJumpFromTrigger()
 {
	 m_waitForJump = false;
	 m_checkCollideJump = false;
	 m_jumpWhenLanding = false;
 }

 float Player::lerp(float a, float b, float f)
 {
	 return a + f * (b - a);
 }

 void Player::speedLines(const Vector3& velocityXZ, const float& velocityY)
 {
	 constexpr float velocityXZModifier = 0.95f;
	 constexpr float velocityYModifier = 0.7f;
	 constexpr float speedLinesThicknessModifier = 0.00012f;
	 constexpr float speedLinesRadiusValue = 1.27f;
	 constexpr float speedLinesSpeedFactor = 1.4f;
	 constexpr float lerpMin = 0.6f;
	 constexpr float maxSpeedLinesRadiusValue = speedLinesRadiusValue - 1.0f;
	 constexpr float speedLinesSpeedFactorValue = speedLinesSpeedFactor - 1.0f;
	 constexpr float speedLinesSpeedLerpModifier = 0.5f;

	 //Speedlines
	 float speedLineThickness = std::clamp(velocityXZ.Length() * velocityXZModifier / m_maxSpeed + std::fabs(velocityY) * velocityYModifier / m_maxYSpeed, 0.0f, 1.0f);
	 if (speedLineThickness > lerpMin)
		 m_sLT = lerp(m_sLT, speedLineThickness, m_frameTime);
	 else
		 m_sLT = speedLineThickness;
	 m_gameObject->getScene()->getGraphics()->setSpeedlineThickness(m_sLT * speedLinesThicknessModifier);

	 float speedlineRadius = std::clamp(speedLinesRadiusValue - m_sLT, maxSpeedLinesRadiusValue, 1.f);
	 if (speedlineRadius > speedLinesRadiusValue * lerpMin)
		 m_sLR = lerp(m_sLR, speedlineRadius, m_frameTime);
	 else
		 m_sLR = speedlineRadius;
	 m_gameObject->getScene()->getGraphics()->setSpeedlineRadius(m_sLR);

	 float speedLineSpeed = speedLinesSpeedFactorValue + m_sLT;
	 if (speedLineSpeed > speedLinesSpeedFactor * lerpMin)
		 m_sLS = lerp(m_sLS, speedLineSpeed, m_frameTime * speedLinesSpeedLerpModifier);
	 else
		 m_sLS = speedLineSpeed;
	 
	 float speedLinesSpeedChanger = 0.0f;
	 if (0.0f < m_sLS && m_sLS < 0.9f)
		 speedLinesSpeedChanger = 0.2f;
	 else if (0.9f < m_sLS && m_sLS < 1.2f)
		 speedLinesSpeedChanger = 0.6f;
	 else if (m_sLS < 1.4f)
		 speedLinesSpeedChanger = 0.8f;

	 m_gameObject->getScene()->getGraphics()->setSpeedlineSpeedFactor(speedLinesSpeedChanger);
 }

 void Player::grappleHook(Vector3 cameraLook)
 {
	 if (Input::getInput().mousePressed(Input::LeftButton))
	 {
		 //cast ray

		 //this belongs in physics but who cares
		 std::vector<Ref<GameObject>> scene = getGameObject()->getScene()->getSceneGameObjects();
		 float dist = 200;
		 bool hitObj = false;
		 for (auto& go : scene)
		 {
			 Ref<OrientedBoxCollider> obb = go->getComponentType<OrientedBoxCollider>(Component::ComponentEnum::ORIENTED_BOX_COLLIDER);
			 if (obb != nullptr)
			 {
				 float d = 10000000000000;
				 if (obb->getInternalCollider().Intersects(m_playerCamera->getCamera()->getPosition(), cameraLook, d))
				 {
					 if (d < dist)
					 {
						 dist = d;
						 hitObj = true;
					 }
				 }
			 }
		 }
		 cameraLook.Normalize(); //vem vet filip kanske inte t�nkte p� det
		 Vector3 attachmentPoint = m_playerCamera->getCamera()->getPosition();
		 if (dist < 10000 && hitObj)
		 {
			 attachmentPoint += dist * cameraLook;

			 //fix camera offset
			 m_hookDist = (attachmentPoint - getTransform()->getPosition()).Length();

			 m_hookPoint = attachmentPoint;
			 m_hooked = true;
			 m_ground = false;
			 m_rb->startPendelMotion(m_hookPoint, m_hookDist);
		 }
		 else
		 {
			 m_hooked = false;
		 }
	 }
	 if (Input::getInput().mouseReleased(Input::LeftButton))
	 {
		 m_hooked = false;
		 m_rb->stopPendelMotion();
	 }
 }
 
 Vector3 Player::moveObjectCheck(Vector3 moveDirection2)
 {
	 if (m_movObj == true)
	 {
		 moveDirection2 -= m_deltaPos;
		 //cast ray
		 constexpr float maxDist = 3.25f;
		 std::vector<Ref<GameObject>> scene = getGameObject()->getScene()->getSceneGameObjects();
		 float dist = FLT_MAX;
		 bool noHit = true;
		 if (m_normal.Length() != 0)
		 {
			 for (auto& go : scene)
			 {
				 Ref<OrientedBoxCollider> obb = go->getComponentType<OrientedBoxCollider>(Component::ComponentEnum::ORIENTED_BOX_COLLIDER);
				 if (obb != nullptr)
				 {
					 float d = 10000000000000;
					 if (obb->getInternalCollider().Intersects(m_playerCamera->getCamera()->getPosition(), -m_normal, d))
					 {
						 if (d < dist) dist = d;
						 noHit = false;
					 }
				 }
			 }
		 }
		 if (dist > maxDist || noHit)
		 {
			 m_movObj = false;
		 }
	 }
	 return moveDirection2;
 }