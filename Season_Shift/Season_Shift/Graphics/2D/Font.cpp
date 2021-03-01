#include "pch.h"
#include "Font.h"

Font::Font(const std::shared_ptr<DXDevice>& dev)
{
}

Font::Font(const std::shared_ptr<DXDevice>& dev, const std::wstring& fontPath)
{
	m_font = dev->createSpriteFont(fontPath);
}

void Font::draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch)
{

}

void Font::setText(const std::string& text)
{
	m_text = text;
}
