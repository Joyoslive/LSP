#pragma once
#include "ISprite.h"
#include "../DX/DXDevice.h"
#include <DirectXColors.h>

class SpriteTexture : public ISprite
{
private:
	std::shared_ptr<DXTexture> m_texture;
	RECT m_rect;
	DirectX::SimpleMath::Vector2 m_correctedPosition;
	float m_rotation;
public:
	SpriteTexture(std::shared_ptr<DXTexture> texture, float rotation);

	void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
	void setColor(const DirectX::SimpleMath::Color& col) override;
};

