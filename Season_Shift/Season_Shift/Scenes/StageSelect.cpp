#include "pch.h"
#include "StageSelect.h"
#include "../Input.h"

StageSelect::StageSelect(Graphics* gph, SceneManager* sm) : Scene(gph, sm)
{}

void StageSelect::setUpScene()
{
	if (m_firstSetup)
	{
		m_firstSetup = false;

		auto resDev = m_graphics->getResourceDevice();
		m_title = resDev->createSprite("Stage Select", L"Textures/Sprites/Fonts/Oleo.spritefont", ScreenPos::MIDDLE, 100.f);
		m_backButton = resDev->createSpriteTexture("Textures/MainMenu/Button.png", 68.f, 650.f, 0.65);
		m_backButtonDown = resDev->createSpriteTexture("Textures/MainMenu/ButtonPressed.png", 68.f, 656.f, 0.65);
		m_backText = resDev->createSprite("Back", L"Textures/Sprites/Fonts/font.spritefont", 127.f, 672.f);

		m_backButtonDown->setShow(false);

		m_backButton->onClick([this]()
			{
				m_backButton->setShow(false);
				m_backButtonDown->setShow(true);
				m_backText->setPosition({m_backText->getPosition().x, m_backText->getPosition().y + 6});
			});
		m_backButtonDown->onRelease([this]()
			{
				if (m_backButton->getClicked())
				{
					m_backButton->setShow(false);
					m_backButtonDown->setShow(false);
					m_backText->setShow(false);
					m_title->setShow(false);

					m_backText->setPosition({m_backText->getPosition().x, m_backText->getPosition().y - 6});

					Input::getInput().lockMouse(2);
					m_sceneManager->changeScene(0); // Switch back to main menu
				}
			});
		m_backButtonDown->onGlobalRelease([this]()
			{
				if (m_backButton->getClicked())
				{
					m_backButton->setShow(true);
					m_backButtonDown->setShow(false);
					m_backText->setPosition({m_backText->getPosition().x, m_backText->getPosition().y - 6});
				}
			});

		m_graphics->addToSpriteBatch(m_title);
		m_graphics->addToSpriteBatch(m_backButton);
		m_graphics->addToSpriteBatch(m_backButtonDown);
		m_graphics->addToSpriteBatch(m_backText);
	}
	else
	{
		m_backButton->setShow(true);
		m_backText->setShow(true);
		m_title->setShow(true);
	}
	
	m_mainCamera = std::make_shared<Camera>(); // Camera in order to be able to renders
}
