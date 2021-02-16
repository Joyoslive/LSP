#pragma once
#include "Logic.h"
#include "Player.h"

class PlayerJumpTrigger : public Logic
{
private:
	Ref<Transform> m_playerTransform; 
	Ref<Transform> m_myTransform;
	Ref<Player> m_player;
	bool m_ground;
public:
	PlayerJumpTrigger(Ref<GameObject> player);
	~PlayerJumpTrigger();

	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
};

