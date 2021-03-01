#include "pch.h"
#include "SpriteRenderer.h"

using namespace DirectX;

SpriteRenderer::SpriteRenderer(DXDevice* dev)
{
	m_spriteBatch = dev->createSpriteBatch();
}

void SpriteRenderer::queueDraw(const std::shared_ptr<ISprite>& sprite)
{
	m_spriteQueue.push(sprite);
}

void SpriteRenderer::queueDraw(std::shared_ptr<ISprite>& sprite, float x, float y)
{
	sprite->setPosition({x, y});
	m_spriteQueue.push(sprite);
}

void SpriteRenderer::drawQueued(DXDevice* dev)
{
	m_spriteBatch->Begin();
	while(m_spriteQueue.size() > 0) {
		m_spriteQueue.front()->draw(m_spriteBatch);
		m_spriteQueue.pop();
	}
	m_spriteBatch->End();
}