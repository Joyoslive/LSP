#pragma once
#include "ISprite.h"
#include "../DX/DXDevice.h"
#include <DirectXColors.h>
#include <functional>

class SpriteTexture : public ISprite
{
private:
	std::shared_ptr<DXTexture> m_texture;
	RECT m_rect;
	float m_rotation;
	float m_depth;
	std::function<void()> m_callback;
public:
	SpriteTexture(std::shared_ptr<DXTexture> texture, float rotation, float m_depth, std::function<void()> callback);

	void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
	void setColor(const DirectX::SimpleMath::Color& col) override;

	void checkForClick(int mouseX, int mouseY, bool isClicked = false) const override;

	float getWidth() const;
	float getHeight() const;

	const DirectX::SimpleMath::Vector2& getPosition() override; 
};

