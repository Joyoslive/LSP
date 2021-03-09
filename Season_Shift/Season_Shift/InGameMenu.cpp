#include "pch.h"
#include "InGameMenu.h"
#include "Graphics/2D/ISprite.h"

//InGameMenu::InGameMenu(std::shared_ptr<ISprite> resume, std::shared_ptr<ISprite> exitToMenu, std::shared_ptr<ISprite> exitGame)
//{
//	m_resume = resume;
//	m_exitToMenu = exitToMenu;
//	m_exitGame = exitGame;
//}

InGameMenu::InGameMenu(std::vector<std::shared_ptr<ISprite>> sprites) :
	m_sprites(sprites)
{
}

void InGameMenu::shouldDraw(bool shouldDraw)
{
	//m_resume->setShow(shouldDraw);
	//m_exitToMenu->setShow(shouldDraw);
	//m_exitGame->setShow(shouldDraw);

	for (auto sprite : m_sprites)
	{
		sprite->setShow(shouldDraw);
	}

	
}
