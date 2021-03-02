#pragma once
#include "ISprite.h"
#include "../DX/DXDevice.h"

class Text : public ISprite
{
private:
	std::shared_ptr<DirectX::SpriteFont> m_font;
	std::string m_text;
	DirectX::SimpleMath::Color m_color;

public:
	Text();
	Text(std::shared_ptr<DirectX::SpriteFont> font);
	~Text() = default;

	void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
	void setText(const std::string& text) override;
	void setFont(std::shared_ptr<DirectX::SpriteFont> font) override;
	void setColor(const DirectX::SimpleMath::Color& col) override;

	std::shared_ptr<DirectX::SpriteFont> getFont();
};

