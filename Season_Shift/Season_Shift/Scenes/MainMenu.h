#pragma once
#include "../Scene.h"
#include "../Graphics/2D/Text.h"
#include "../Graphics/2D/SpriteTexture.h"

class MainMenu : public Scene
{
private:
	std::shared_ptr<Text> m_title;
	//buttons
	std::shared_ptr<SpriteTexture> m_background;
public:
	MainMenu(Graphics* graphics);
	~MainMenu() = default;
	void setUpScene() override;
};

