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
	virtual inline const DirectX::SimpleMath::Vector2& getPosition() { return m_position; }
	inline void setScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	inline void setShow(bool show) { m_show = show; }
	inline void setWindowSize(DirectX::SimpleMath::Vector2 windowSize) { m_windowSize = windowSize; }

	virtual void checkForClick(int mouseX, int mouseY, bool isClicked = false) const {};

	inline DirectX::SimpleMath::Vector2 getCorrectScaleVector(DirectX::SimpleMath::Vector2 changeVector) { return { changeVector.x * m_windowSize.x / m_baseWindowSize.x, changeVector.y * m_windowSize.y / m_baseWindowSize.y }; };
	virtual inline std::shared_ptr<DirectX::SpriteFont> getFont() { return std::shared_ptr<DirectX::SpriteFont>(); }
	inline bool getShow() const { return m_show; }
	
protected:
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_correctedPosition;
	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_correctedScale;
	DirectX::SimpleMath::Vector2 m_windowSize;
	DirectX::SimpleMath::Vector2 m_baseWindowSize = { 1280.0f, 720.0f };
	bool m_show;
};

