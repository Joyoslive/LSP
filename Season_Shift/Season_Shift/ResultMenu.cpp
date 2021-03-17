#include "pch.h"
#include "ResultMenu.h"
#include "Graphics/2D/ISprite.h"
#include "Scene.h"
#include "Input.h"
#include "SceneManager.h"

ResultMenu::ResultMenu(std::vector<std::shared_ptr<ISprite>> sprites, std::shared_ptr<ISprite> gradeSprite, std::shared_ptr<ISprite> timeSprite, SceneManager* sceneManager)
{
	m_sprites = sprites;
	m_gradeText = gradeSprite;
	m_timeText = timeSprite;
	m_active = false;
	m_scene = nullptr;
	m_sceneManager = sceneManager;
}

ResultMenu::~ResultMenu()
{
	m_sprites.clear();
	m_scene = nullptr;
	m_sceneManager = nullptr;
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
	sprintf_s(msgbuf, "Your Grade was %s", grade.c_str());
	m_gradeText->setText(std::string(msgbuf));

	int minutes = (int)fabs(time) / 60;
	int seconds = (int)fabs(time) % 60;
	std::string text = "Your Time was ";

	std::string minutesText = "";
	if (minutes < 10)
		minutesText += "0";
	minutesText += std::to_string(minutes);

	std::string secondsText = "";
	if (seconds < 10)
		secondsText += "0";
	secondsText += std::to_string(seconds);

	text += minutesText + ":" + secondsText;
	m_timeText->setText(text);

	setShowSprites(true);
	m_scene->setIsPaused(true);
	m_active = true;
	Input::getInput().lockMouse(2);
}

void ResultMenu::retryLevel()
{
	m_scene->setIsPaused(false);
	setShowSprites(false);
	m_active = false;
	Input::getInput().lockMouse(1);
}

void ResultMenu::nextLevel()
{
	m_scene->setIsPaused(false);
	Input::getInput().lockMouse(1);
	setShowSprites(false);
	m_active = false;
	m_sceneManager->changeScene(1);
}

void ResultMenu::updateScene(Scene* activeScene)
{
	m_scene = activeScene;
}

bool ResultMenu::getActive() const
{
	return m_active;
}