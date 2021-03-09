#pragma once
#include <functional>
#include <vector>

class ISprite;

class InGameMenu
{
private:
	std::vector<std::shared_ptr<ISprite>> m_sprites;

	std::shared_ptr<ISprite> m_resume;
	std::shared_ptr<ISprite> m_exitToMenu;
	std::shared_ptr<ISprite> m_exitGame;
public:
	//InGameMenu(std::shared_ptr<ISprite> resume, std::shared_ptr<ISprite> exitToMenu, std::shared_ptr<ISprite> exitGame);
	InGameMenu(std::vector<std::shared_ptr<ISprite>> sprites);
	~InGameMenu() = default;

	void shouldDraw(bool shouldDraw);


};

