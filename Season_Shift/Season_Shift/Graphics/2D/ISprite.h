#pragma once
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

class ISprite
{
public:
	virtual void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) = 0;
	// Only applicable to Font (probably, but we want this exposed)
	virtual inline void setText(const std::string& text) { };
	virtual inline void setFont(std::shared_ptr<DirectX::SpriteFont> font) { };
	virtual void setColor(const DirectX::SimpleMath::Color& col) = 0;
	inline void setPosition(const DirectX::SimpleMath::Vector2 position) { m_position = position; }
	
protected:
	DirectX::SimpleMath::Vector2 m_position;
};

