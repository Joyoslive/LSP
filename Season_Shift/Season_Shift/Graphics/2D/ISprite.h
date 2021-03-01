#pragma once
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

class ISprite
{
public:
	virtual void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) = 0;
	// Only applicable to Font (probably, but we want his exposed)
	virtual void setText(const std::string& text) { };
	void setPosition(const DirectX::SimpleMath::Vector2 position) { m_position = position; }
protected:
	DirectX::SimpleMath::Vector2 m_position;
};

