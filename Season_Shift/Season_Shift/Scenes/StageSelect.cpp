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

		m_stage1ButtonText = resDev->createSprite("Spring Stage", L"Textures/Sprites/Fonts/font.spritefont", ScreenPos::MIDDLE, ScreenPos::MIDDLE);
		auto pos = m_stage1ButtonText->getPosition();
		pos.y += 65;
		m_stage1ButtonText->setPosition(pos);
		m_stage1Button = resDev->createSpriteTexture("Textures/MainMenu/Button.png", pos.x - 90, pos.y - 20);
		m_stage1ButtonDown = resDev->createSpriteTexture("Textures/MainMenu/ButtonPressed.png", pos.x - 90, pos.y - 14);

		m_stage2ButtonText = resDev->createSprite("Fall Stage", L"Textures/Sprites/Fonts/font.spritefont", ScreenPos::MIDDLE, ScreenPos::MIDDLE);
		pos = m_stage2ButtonText->getPosition();
		pos.y += 125;
		m_stage2ButtonText->setPosition(pos);
		m_stage2Button = resDev->createSpriteTexture("Textures/MainMenu/Button.png", pos.x - 90, pos.y - 20);
		m_stage2ButtonDown = resDev->createSpriteTexture("Textures/MainMenu/ButtonPressed.png", pos.x - 90, pos.y - 14);

		m_stage1ButtonDown->setShow(false);
		m_stage2ButtonDown->setShow(false);
		m_backButtonDown->setShow(false);

		m_stage1Button->onClick([this]()
			{
				m_stage1Button->setShow(false);
				m_stage1ButtonDown->setShow(true);
				m_stage1ButtonText->setPosition({ m_stage1ButtonText->getPosition().x, m_stage1ButtonText->getPosition().y + 6 });
			});
		m_stage1ButtonDown->onRelease([this]()
			{
				if (m_stage1Button->getClicked())
				{
					m_backButton->setShow(false);
					m_backButtonDown->setShow(false);
					m_backText->setShow(false);
					m_title->setShow(false);
					m_stage1Button->setShow(false);
					m_stage1ButtonDown->setShow(false);
					m_stage1ButtonText->setShow(false);
					m_stage2Button->setShow(false);
					m_stage2ButtonDown->setShow(false);
					m_stage2ButtonText->setShow(false);

					m_stage1ButtonText->setPosition({ m_stage1ButtonText->getPosition().x, m_stage1ButtonText->getPosition().y - 6 });

					Input::getInput().lockMouse(0);

					m_sceneManager->changeScene(6); // Switch
				}
			});
		m_stage1ButtonDown->onGlobalRelease([this]()
			{
				if (m_stage1Button->getClicked())
				{
					m_stage1Button->setShow(true);
					m_stage1ButtonDown->setShow(false);
					m_stage1ButtonText->setPosition({ m_stage1ButtonText->getPosition().x, m_stage1ButtonText->getPosition().y - 6 });
				}
			});
		m_stage2Button->onClick([this]()
			{
				m_stage2Button->setShow(false);
				m_stage2ButtonDown->setShow(true);
				m_stage2ButtonText->setPosition({ m_stage2ButtonText->getPosition().x, m_stage2ButtonText->getPosition().y + 6 });
			});
		m_stage2ButtonDown->onRelease([this]()
			{
				if (m_stage2Button->getClicked())
				{
					m_backButton->setShow(false);
					m_backButtonDown->setShow(false);
					m_backText->setShow(false);
					m_title->setShow(false);
					m_stage1Button->setShow(false);
					m_stage1ButtonDown->setShow(false);
					m_stage1ButtonText->setShow(false);
					m_stage2Button->setShow(false);
					m_stage2ButtonDown->setShow(false);
					m_stage2ButtonText->setShow(false);

					m_stage2ButtonText->setPosition({ m_stage2ButtonText->getPosition().x, m_stage2ButtonText->getPosition().y - 6 });

					Input::getInput().lockMouse(0);

					m_sceneManager->changeScene(4); // Switch
				}
			});
		m_stage2ButtonDown->onGlobalRelease([this]()
			{
				if (m_stage2Button->getClicked())
				{
					m_stage2Button->setShow(true);
					m_stage2ButtonDown->setShow(false);
					m_stage2ButtonText->setPosition({ m_stage2ButtonText->getPosition().x, m_stage2ButtonText->getPosition().y - 6 });
				}
			});

		m_backButton->onClick([this]()
			{
				m_backButton->setShow(false);
				m_backButtonDown->setShow(true);
				m_backText->setPosition({ m_backText->getPosition().x, m_backText->getPosition().y + 6 });
			});
		m_backButtonDown->onRelease([this]()
			{
				if (m_backButton->getClicked())
				{
					m_backButton->setShow(false);
					m_backButtonDown->setShow(false);
					m_backText->setShow(false);
					m_title->setShow(false);
					m_stage1Button->setShow(false);
					m_stage1ButtonDown->setShow(false);
					m_stage1ButtonText->setShow(false);
					m_stage2Button->setShow(false);
					m_stage2ButtonDown->setShow(false);
					m_stage2ButtonText->setShow(false);

					m_backText->setPosition({ m_backText->getPosition().x, m_backText->getPosition().y - 6 });

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
					m_backText->setPosition({ m_backText->getPosition().x, m_backText->getPosition().y - 6 });
				}
			});


		m_graphics->addToSpriteBatch(m_title);
		m_graphics->addToSpriteBatch(m_backButton);
		m_graphics->addToSpriteBatch(m_backButtonDown);
		m_graphics->addToSpriteBatch(m_backText);
		m_graphics->addToSpriteBatch(m_stage1Button);
		m_graphics->addToSpriteBatch(m_stage1ButtonDown);
		m_graphics->addToSpriteBatch(m_stage1ButtonText);
		m_graphics->addToSpriteBatch(m_stage2Button);
		m_graphics->addToSpriteBatch(m_stage2ButtonDown);
		m_graphics->addToSpriteBatch(m_stage2ButtonText);

	}
	else
	{
		m_backButton->setShow(true);
		m_backText->setShow(true);
		m_title->setShow(true);

		m_stage1ButtonText->setShow(true);
		m_stage1Button->setShow(true);

		m_stage2ButtonText->setShow(true);
		m_stage2Button->setShow(true);
	}

	m_mainCamera = std::make_shared<Camera>(); // Camera in order to be able to renders
}
