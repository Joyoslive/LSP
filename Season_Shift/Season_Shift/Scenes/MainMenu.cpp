#include "pch.h"
#include "MainMenu.h"
#include "../SceneManager.h"
#include "../Input.h"
#include "../Sound.h"

MainMenu::MainMenu(Graphics* graphics, SceneManager* sm, const Window* const win) : Scene(graphics, sm)
{
	m_win = win;
}

void MainMenu::setUpScene()
{
	if (m_firstSetup)
	{
		m_firstSetup = false;
		Input::getInput().lockMouse(2);

		auto resDev = m_graphics->getResourceDevice();
		m_title = resDev->createSprite("Season Shift", L"Textures/Sprites/Fonts/Oleo.spritefont",
			ScreenPos::MIDDLE, 100.0f);
		m_titleOutline = resDev->createSprite("Season Shift", L"Textures/Sprites/Fonts/OleoBig.spritefont",
			ScreenPos::MIDDLE, 100.0f);
		m_title = resDev->createSprite("Made by Sam Axelsson,\n Gunnar Cerne, Filip Eriksson,\n Nadif Ginola, Jonatan Hermansson ", L"Textures/Sprites/Fonts/Oleo.spritefont",
			ScreenPos::MIDDLE, 250.0f);
		m_title->setColor(DirectX::SimpleMath::Color(0, 1, 0, 1));
		m_titleOutline->setColor(DirectX::SimpleMath::Color(0, 0.5, 0, 1));

		m_background = resDev->createSpriteTexture("Textures/MainMenu/background_2.jpg", 0, 0, 0.6667f, 0.6667f);

		//Buttons
		m_playText = resDev->createSprite("Play", L"Textures/Sprites/Fonts/font.spritefont", ScreenPos::MIDDLE, ScreenPos::MIDDLE);
		auto pos = m_playText->getPosition();
		pos.y += 65;
		m_playText->setPosition(pos);
		std::shared_ptr<ISprite> pressedPlayButton = resDev->createSpriteTexture("Textures/MainMenu/ButtonPressed.png", pos.x - 90, pos.y - 14);
		pressedPlayButton->setShow(false);
		std::shared_ptr<ISprite> playButton = resDev->createSpriteTexture("Textures/MainMenu/Button.png", pos.x - 90, pos.y - 20);
		playButton->onClick([this]()
			{
				m_buttons[0]->setShow(false);
				m_buttons[1]->setShow(true);
				m_playText->setPosition({m_playText->getPosition().x, m_playText->getPosition().y + 6});
			});
		pressedPlayButton->onRelease([this]()
			{
				if (m_buttons[0]->getClicked())
				{
					m_buttons[0]->setShow(true);
					m_buttons[1]->setShow(false);
					m_playText->setPosition({m_playText->getPosition().x, m_playText->getPosition().y - 6});
					SwitchScene(Scenes::STAGE_SELECT);
				}
			});
		pressedPlayButton->onGlobalRelease([this]()
			{
				if (m_buttons[0]->getClicked())
				{
					m_buttons[1]->setShow(false);
					m_buttons[0]->setShow(true);
					m_playText->setPosition({m_playText->getPosition().x, m_playText->getPosition().y - 6});
				}
			});

		m_quitText = resDev->createSprite("Quit", L"Textures/Sprites/Fonts/font.spritefont", ScreenPos::MIDDLE, ScreenPos::MIDDLE);
		pos = m_quitText->getPosition();
		pos.y += 125;
		m_quitText->setPosition(pos);
		std::shared_ptr<ISprite> pressedExitButton = resDev->createSpriteTexture("Textures/MainMenu/ButtonPressed.png", pos.x - 90, pos.y - 14);
		pressedExitButton->setShow(false);
		std::shared_ptr<ISprite> exitButton = resDev->createSpriteTexture("Textures/MainMenu/Button.png", pos.x - 90, pos.y - 20);
		exitButton->onClick([this]()
			{
				m_buttons[2]->setShow(false);
				m_buttons[3]->setShow(true);
				m_quitText->setPosition({m_quitText->getPosition().x, m_quitText->getPosition().y + 6});
			});
		pressedExitButton->onRelease([this]()
			{
				if (m_buttons[2]->getClicked())
				{
					m_sceneManager->quitGame();
				}
			});
		pressedExitButton->onGlobalRelease([this]()
			{
				if (m_buttons[2]->getClicked())
				{
					m_buttons[3]->setShow(false);
					m_buttons[2]->setShow(true);
					m_quitText->setPosition({m_quitText->getPosition().x, m_quitText->getPosition().y - 6});
				}
			});


		m_buttons.push_back(playButton);
		m_buttons.push_back(pressedPlayButton);
		m_buttons.push_back(exitButton);
		m_buttons.push_back(pressedExitButton);

		// Add to sprite batch - Order matters
		m_graphics->addToSpriteBatch(m_background);
		m_graphics->addToSpriteBatch(playButton);
		m_graphics->addToSpriteBatch(pressedPlayButton);
		m_graphics->addToSpriteBatch(m_playText);
		m_graphics->addToSpriteBatch(exitButton);
		m_graphics->addToSpriteBatch(pressedExitButton);
		m_graphics->addToSpriteBatch(m_quitText);
		m_graphics->addToSpriteBatch(m_titleOutline);
		m_graphics->addToSpriteBatch(m_title);

		//Music
		std::vector<std::string> v;
		v.push_back("Sounds/Menu.wav");
		v.push_back("Sounds/button.wav");
		m_music.start(v);
		m_music.playLoop("Sounds/Menu.wav");
	}
	else
	{
		m_music.playLoop("Sounds/Menu.wav");
		m_background->setShow(true);
		m_playText->setShow(true);
		m_quitText->setShow(true);
		m_title->setShow(true);
		m_titleOutline->setShow(true);
		m_buttons[0]->setShow(true);
		m_buttons[2]->setShow(true);
	}

	m_mainCamera = std::make_shared<Camera>(); // We need a camera in order to display anything
}

void MainMenu::SwitchScene(Scenes scene)
{
	m_music.play("Sounds/button.wav");
	m_background->setShow(false);
	m_playText->setShow(false);
	m_quitText->setShow(false);
	m_title->setShow(false);
	m_titleOutline->setShow(false);
	for (auto& b : m_buttons)
		b->setShow(false);
	int idx = (int)scene;
	m_sceneManager->changeScene(idx);
	m_music.stopLoop();
}