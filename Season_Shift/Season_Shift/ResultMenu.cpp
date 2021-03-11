#include "pch.h"
#include "ResultMenu.h"
#include "Graphics/2D/ISprite.h"

ResultMenu::ResultMenu(std::vector<std::shared_ptr<ISprite>> sprites, std::shared_ptr<ISprite> textSprite) : m_sprites(sprites), m_text(textSprite)
{
	
}

ResultMenu::~ResultMenu()
{
	m_sprites.clear();
}

void ResultMenu::setShowSprites(bool show)
{
	for (int i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->setShow(show);
	}
}

void ResultMenu::playerReachedGoal(long double time, const std::string& grade)
{
	char msgbuf[1000];
	sprintf_s(msgbuf, "Your Grade was %s\nYour Time was %d", grade.c_str(), (int)fabs(time));
	m_text->setText(std::string(msgbuf));
	setShowSprites(true);
}