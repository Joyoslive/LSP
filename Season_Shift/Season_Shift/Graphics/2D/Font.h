#pragma once
#include "ISprite.h"
#include "../DX/DXDevice.h"

class Font : public ISprite
{
private:
	std::shared_ptr<DirectX::SpriteFont> m_font;
	std::string m_text;

public:
	Font(const std::shared_ptr<DXDevice>& dev);
	Font(const std::shared_ptr<DXDevice>& dev, const std::wstring& fontPath);
	~Font() = default;

	void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
	void setText(const std::string& text) override;
};

