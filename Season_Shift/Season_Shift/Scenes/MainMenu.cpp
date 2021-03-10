#include "pch.h"
#include "MainMenu.h"
#include "../SceneManager.h"

MainMenu::MainMenu(Graphics* graphics, SceneManager* sm) : Scene(graphics, sm)
{}

void MainMenu::setUpScene()
{
	m_graphics->clearSpriteBatch();

	auto resDev = m_graphics->getResourceDevice();
	m_title = resDev->createSprite("Season Shift", L"Textures/Sprites/Fonts/font.spritefont",
		ScreenPos::MIDDLE, 72.0f);

	m_background = resDev->createSpriteTexture("Textures/MainMenu/background.jpg", 0, 0, 2, 2);
	
	//Buttons
	auto dummy = resDev->createSprite("", L"Textures/Sprites/Fonts/font.spritefont", ScreenPos::MIDDLE, ScreenPos::MIDDLE);
	auto pos = dummy->getPosition();
	auto playButton = resDev->createSpriteTexture("Textures/MainMenu/Button.png", pos.x - 72.0, pos.y, 4, 1, 0, 0, 
		[this](){ SwitchScene(Scenes::STAGE_SELECT); });
	m_buttons.push_back(playButton);
	
	m_graphics->addToSpriteBatch(m_background);
	m_graphics->addToSpriteBatch(playButton);
	m_graphics->addToSpriteBatch(m_title);
}

void MainMenu::SwitchScene(Scenes scene)
{
	int idx = (int)scene;
	m_sceneManager->changeScene(idx);
	m_graphics->clearSpriteBatch();
}