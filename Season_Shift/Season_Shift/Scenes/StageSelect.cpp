#include "pch.h"
#include "StageSelect.h"

StageSelect::StageSelect(Graphics* gph, SceneManager* sm) : Scene(gph, sm)
{}

void StageSelect::setUpScene()
{
	auto resDev = m_graphics->getResourceDevice();
	m_title = resDev->createSprite("Stage Select", L"Textures/Sprites/Fonts/Oleo.spritefont", ScreenPos::MIDDLE, 100.f);
	m_backButton = resDev->createSpriteTexture("Textures/MainMenu/Button.png", 72.f, 680.f);
	m_backText = resDev->createSprite("Back", L"Textures/Sprites/Fonts/font.spritefont", 72.f, 680.f);

	m_graphics->addToSpriteBatch(m_title);
	m_graphics->addToSpriteBatch(m_backButton);
	m_graphics->addToSpriteBatch(m_backText);
}
