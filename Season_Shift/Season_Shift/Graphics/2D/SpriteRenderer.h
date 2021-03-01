#pragma once
#include "ISprite.h"
#include "../DX/DXDevice.h"
#include <queue>

class SpriteRenderer
{
public:
private:
	struct DrawInfo
	{
		std::shared_ptr<ISprite> sprite;
		DirectX::SimpleMath::Vector2 pos;
	};
private:
	std::shared_ptr<DXDevice> m_dev;
	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::queue<std::shared_ptr<ISprite>> m_spriteQueue;
	bool m_drawing;

public:
	SpriteRenderer(std::shared_ptr<DXDevice> dev);
	~SpriteRenderer() = default;

	void queueDraw(const std::shared_ptr<ISprite>& sprite);

	// Warning: Will change the position in the internal state of the sprite!
	void queueDraw(std::shared_ptr<ISprite>& sprite, float x, float y);

	void drawQueued();
};

