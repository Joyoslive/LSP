#pragma once
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

class ISprite
{
public:
	virtual void draw(const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) = 0;
	// Only applicable to Font (probably, but we want this exposed)
	virtual inline void setText(const std::string& text) { }
	virtual inline void setFont(std::shared_ptr<DirectX::SpriteFont> font) { }
	virtual void setColor(const DirectX::SimpleMath::Color& col) = 0;
	inline void setPosition(const DirectX::SimpleMath::Vector2 position) { m_position = position; }
	inline const DirectX::SimpleMath::Vector2& getPosition() { return m_position; }
	inline void setScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	inline void setShow(bool show) { m_show = show; }
	// Use sparingly
	inline void setClicked(bool clicked) { m_clicked = clicked; }

	virtual void onClick(std::function<void()> callback) {};
	virtual void onRelease(std::function<void()> callback) {};

	virtual void checkForClick(int mouseX, int mouseY, bool isClicked = false) {};
	virtual void checkForRelease(int mouseX, int mouseY, bool mouseReleased = false) {};

	virtual inline std::shared_ptr<DirectX::SpriteFont> getFont() { return std::shared_ptr<DirectX::SpriteFont>(); }
	inline bool getShow() { return m_show; }
	inline bool getClicked() { return m_clicked; }
	
protected:
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_scale;
	bool m_show;
	bool m_clicked;
};

