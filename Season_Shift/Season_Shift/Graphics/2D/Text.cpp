#include "pch.h"
#include "Text.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Text::Text()
{
	m_show = true;
	m_color = Color(1, 1, 1, 1);
	m_text = "";
}

Text::Text(std::shared_ptr<DirectX::SpriteFont> font)
{
	m_show = true;
	m_font = font;
	m_color = Color(1, 1, 1, 1);
	m_text = "";
	//Divide by sqrt(2) so scale's length is 1
	m_scale = DirectX::SimpleMath::Vector2(1.0f, 1.0f) * 1.0f / std::sqrtf(2);;
}

void Text::draw(const std::shared_ptr<SpriteBatch>& spriteBatch)
{
	if (m_show)
	{
		auto origin = m_font->MeasureString(m_text.c_str()) / 2.f;
		m_correctedPosition = getCorrectScaleVector(m_position);
		m_correctedScale = getCorrectScaleVector(m_scale);
		m_font->DrawString(spriteBatch.get(), m_text.c_str(), m_correctedPosition, m_color, 0.f, origin, m_correctedScale.Length());
	}
}

void Text::setText(const std::string& text)
{
	m_text = text;
}

void Text::setFont(std::shared_ptr<DirectX::SpriteFont> font)
{
	m_font = font;
}

void Text::setColor(const DirectX::SimpleMath::Color& col)
{
	m_color = col;
}

const DirectX::SimpleMath::Vector2& Text::getPosition()
{
	return m_correctedPosition;
}

const DirectX::SimpleMath::Vector2& Text::getScale()
{
	return m_correctedScale;
}

std::shared_ptr<DirectX::SpriteFont> Text::getFont()
{
	return m_font;
}
