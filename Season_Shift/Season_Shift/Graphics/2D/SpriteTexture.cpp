#include "pch.h"
#include "SpriteTexture.h"

SpriteTexture::SpriteTexture(std::shared_ptr<DXTexture> texture, float rotation, float depth)
{
	m_show = true;
	m_texture = texture;
	m_rect = { 0 };
	m_rotation = rotation;
	m_depth = depth;
}

void SpriteTexture::draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch)
{
	if (m_show)
	{
		m_correctedPosition.x = m_position.x - m_scale.x * m_texture->getDesc().desc2D.Width/2;
		m_correctedPosition.y = m_position.y - m_scale.y * m_texture->getDesc().desc2D.Height/2;
		m_rect.right = m_texture->getDesc().desc2D.Width;
		m_rect.bottom = m_texture->getDesc().desc2D.Height;
		spriteBatch->Draw(m_texture->getSRV().Get(), m_correctedPosition, &m_rect, DirectX::Colors::White, m_rotation, {0,0}, m_scale, DirectX::SpriteEffects_None, m_depth);
	}
}

void SpriteTexture::setColor(const DirectX::SimpleMath::Color& col)
{
	
}

float SpriteTexture::getWidth() const
{
	return m_scale.x * m_texture->getDesc().desc2D.Width;
}

float SpriteTexture::getHeight() const
{
	return m_scale.y * m_texture->getDesc().desc2D.Height;
}