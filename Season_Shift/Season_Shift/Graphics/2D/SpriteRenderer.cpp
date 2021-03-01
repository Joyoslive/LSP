#include "pch.h"
#include "SpriteRenderer.h"

using namespace DirectX;

SpriteRenderer::SpriteRenderer(std::shared_ptr<DXDevice> dev) 
	: m_dev(dev)
{
	m_spriteBatch = m_dev->createSpriteBatch();
}

void SpriteRenderer::queueDraw(const std::shared_ptr<ISprite>& sprite)
{

}

void SpriteRenderer::queueDraw(std::shared_ptr<ISprite>& sprite, float x, float y)
{
	sprite->setPosition({x, y});
	m_spriteQueue.push(sprite);
}

void SpriteRenderer::drawQueued()
{

}
