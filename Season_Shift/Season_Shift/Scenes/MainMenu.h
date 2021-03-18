#pragma once
#include "../Scene.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/2D/Text.h"
#include "../Graphics/2D/SpriteTexture.h"
#include "../Sound.h"
#include "../Window.h"


class MainMenu : public Scene
{
private:
	enum class Scenes
	{
		STAGE_SELECT = 1,
		STAGE_1 = 6,
		STAGE_2 = 7,
	};
private:
	std::shared_ptr<ISprite> m_title;
	std::shared_ptr<ISprite> m_titleOutline;
	std::shared_ptr<ISprite> m_credits;
	std::shared_ptr<ISprite> m_playText;
	std::shared_ptr<ISprite> m_quitText;
	std::shared_ptr<ISprite> m_fullScreenText;
	std::vector<std::shared_ptr<ISprite>> m_buttons;
	std::shared_ptr<ISprite> m_background;
	Sound m_music;
	const Window* m_win;

	bool m_firstSetup = true;
private:
	void SwitchScene(Scenes scene);

public:
	MainMenu(Graphics* graphics, SceneManager* sm, const Window* const win);
	~MainMenu() = default;
	void setUpScene() override;
};

