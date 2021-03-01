#include "pch.h"
#include "Text.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Text::Text()
{
	m_color = Color(1, 1, 1, 1);
	m_text = "";
}

Text::Text(std::shared_ptr<DirectX::SpriteFont> font)
{
	m_font = font;
	m_color = Color(1, 1, 1, 1);
	m_text = "";

}

void Text::draw(const std::shared_ptr<SpriteBatch>& spriteBatch)
{
	auto origin = m_font->MeasureString(m_text.c_str()) / 2.f;
	m_font->DrawString(spriteBatch.get(), m_text.c_str(), m_position, m_color, 0.f, origin);
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
