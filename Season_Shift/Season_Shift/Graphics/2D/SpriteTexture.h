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
	std::function<void()> m_releaseCallback;
	std::function<void()> m_globReleaseCallback;
public:
	SpriteTexture(std::shared_ptr<DXTexture> texture, float rotation, float m_depth, std::function<void()> callback);

	void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
	void setColor(const DirectX::SimpleMath::Color& col) override;

	void onClick(std::function<void()> callback) override;
	void onRelease(std::function<void()> callback) override;
	void onGlobalRelease(std::function<void()> callback) override;

	void checkForClick(int mouseX, int mouseY, bool isClicked = false) override;
	void checkForRelease(int mouseX, int mouseY, bool mouseReleased = false) override;
	void globalRelease() override;

	float getWidth() const;
	float getHeight() const;

	const DirectX::SimpleMath::Vector2& getPosition() override; 
	const DirectX::SimpleMath::Vector2& getScale() override;
};

