#pragma once
#include "../Scene.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/2D/Text.h"
#include "../Graphics/2D/SpriteTexture.h"


class MainMenu : public Scene
{
private:
	enum class Scenes
	{
		STAGE_SELECT = 6,
		STAGE_1 = 4,
		STAGE_2 = 5,
	};
private:
	std::shared_ptr<ISprite> m_title;
	std::shared_ptr<ISprite> m_playText;
	std::shared_ptr<ISprite> m_quitText;
	std::vector<std::shared_ptr<ISprite>> m_buttons;
	std::shared_ptr<ISprite> m_background;

private:
	void SwitchScene(Scenes scene);

public:
	MainMenu(Graphics* graphics, SceneManager* sm);
	~MainMenu() = default;
	void setUpScene() override;
};

