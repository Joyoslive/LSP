#include "pch.h"
#include "SpriteTexture.h"

SpriteTexture::SpriteTexture(std::shared_ptr<DXTexture> texture, float rotation, float depth, std::function<void()> callback)
{
	m_callback = callback;
	m_releaseCallback = nullptr;
	m_globReleaseCallback = nullptr;
	m_show = true;
	m_texture = texture;
	m_rect = { 0 };
	m_rotation = rotation;
	m_depth = depth;
	m_clicked = false;
}

void SpriteTexture::draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch)
{
	if (m_show)
	{
		m_rect.right = m_texture->getDesc().desc2D.Width;
		m_rect.bottom = m_texture->getDesc().desc2D.Height;
		m_correctedScale = getCorrectScaleVector(m_scale);
		m_correctedPosition = getCorrectScaleVector(m_position);
		spriteBatch->Draw(m_texture->getSRV().Get(), m_correctedPosition, &m_rect, DirectX::Colors::White, m_rotation, {0,0}, m_correctedScale, DirectX::SpriteEffects_None, m_depth);
	}
}

void SpriteTexture::setColor(const DirectX::SimpleMath::Color& col)
{
	
}

void SpriteTexture::onClick(std::function<void()> callback)
{
	m_callback = callback;
}

void SpriteTexture::onRelease(std::function<void()> callback)
{
	m_releaseCallback = callback;
}

void SpriteTexture::onGlobalRelease(std::function<void()> callback)
{
	m_globReleaseCallback = callback;
}

void SpriteTexture::checkForClick(int mouseX, int mouseY, bool isClicked)
{
	if (isClicked && m_callback &&
		mouseX > m_correctedPosition.x && mouseX < (m_correctedPosition.x + getWidth()) &&
		mouseY > m_correctedPosition.y && mouseY < (m_correctedPosition.y + getHeight()))
	{
		m_callback();
		m_clicked = true;
	}
}

void SpriteTexture::checkForRelease(int mouseX, int mouseY, bool mouseReleased)
{
	if (mouseReleased && m_releaseCallback &&
		mouseX > m_position.x && mouseX < (m_position.x + getWidth()) &&
		mouseY > m_position.y && mouseY < (m_position.y + getHeight()))
	{
		m_releaseCallback();
		m_clicked = false;
	}
}

void SpriteTexture::globalRelease()
{
	if(m_globReleaseCallback)
		m_globReleaseCallback();

	m_clicked = false;
}

float SpriteTexture::getWidth() const
{
	return m_correctedScale.x * m_texture->getDesc().desc2D.Width;
}

float SpriteTexture::getHeight() const
{
	return m_correctedScale.y * m_texture->getDesc().desc2D.Height;
}

const DirectX::SimpleMath::Vector2& SpriteTexture::getPosition()
{
	return m_correctedPosition;
}

const DirectX::SimpleMath::Vector2& SpriteTexture::getScale()
{
	return m_correctedScale;
}