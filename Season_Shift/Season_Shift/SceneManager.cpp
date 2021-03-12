#include "pch.h"
#include "SceneManager.h"
#include "Scenes/Scene1.h"
#include "Scenes/Scene2.h"
#include "Scenes/Scene3.h"
#include "Scenes/Scene4.h"
#include "Scenes/Scene5.h"
#include "Scenes/MainMenu.h"
#include "InGameMenu.h"
#include "ResultMenu.h"

#include <vector>
#include <assert.h>

SceneManager::SceneManager(Graphics *graphics, const Window* const win)
{
	m_timeToQuit = false;
	m_scenes.emplace_back(std::make_shared<MainMenu>(graphics, this, win));
	createMenu(graphics);
	createScenes(graphics);
}

SceneManager::~SceneManager()
{
	for (int i = 0; i < m_scenes.size(); ++i)
	{
		m_scenes[i]->emptyScene();
	}
	m_scenes.clear();
	m_activeScene = nullptr;
}

void SceneManager::createMenu(Graphics* graphics)
{
	auto menuBack = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-long.jpg", 440, 160, 1.f, 0.7f, 0.f, 1.f);
	graphics->addToSpriteBatch(menuBack);
	menuBack->setShow(false);

	int buttonsX = 520;
	auto clickableSprite = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-rect.jpg", buttonsX, 200, 0.6f, 0.4f, 0.f, 0.5f,
		[this]() { 
		m_activeScene->setPauseState(false); 
		m_activeScene->setIsPaused(false);
	});
	graphics->addToSpriteBatch(clickableSprite);
	clickableSprite->setShow(false);
	auto text1 = graphics->getResourceDevice()->createSprite("Resume", L"Textures/Sprites/Fonts/font.spritefont", buttonsX + 115, 245);
	graphics->addToSpriteBatch(text1);
	text1->setShow(false);

	auto clickableSprite2 = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-rect.jpg", buttonsX, 320, 0.6f, 0.4f, 0.f, 0.5f,
		[this]() { 
		m_activeScene->setAnotherIsPaused(false);
		m_menu->shouldDraw(false);
		changeScene(0); 
	
	});
	graphics->addToSpriteBatch(clickableSprite2);
	clickableSprite2->setShow(false);
	auto text2 = graphics->getResourceDevice()->createSprite("Exit to Menu", L"Textures/Sprites/Fonts/font.spritefont", buttonsX + 115, 365);
	graphics->addToSpriteBatch(text2);
	text2->setScale(DirectX::SimpleMath::Vector2(0.45, 0.45));
	text2->setShow(false);

	auto clickableSprite3 = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-rect.jpg", buttonsX, 440, 0.6f, 0.4f, 0.f, 0.5f,
		[this]() { m_timeToQuit = true; });

	graphics->addToSpriteBatch(clickableSprite3);
	clickableSprite3->setShow(false);
	auto text3 = graphics->getResourceDevice()->createSprite("Exit Game", L"Textures/Sprites/Fonts/font.spritefont", buttonsX + 115, 485);
	graphics->addToSpriteBatch(text3);
	text3->setScale(DirectX::SimpleMath::Vector2(0.5, 0.5));
	text3->setShow(false);

	auto showGradeSprite = graphics->getResourceDevice()->createSprite("Time", L"Textures/Sprites/Fonts/font.spritefont", 1280 / 2, 200);
	graphics->addToSpriteBatch(showGradeSprite);
	showGradeSprite->setScale(DirectX::SimpleMath::Vector2(0.7f, 0.7f));
	showGradeSprite->setShow(false);

	auto showTimeSprite = graphics->getResourceDevice()->createSprite("Time", L"Textures/Sprites/Fonts/font.spritefont", 1280 / 2, 240);
	graphics->addToSpriteBatch(showTimeSprite);
	showTimeSprite->setScale(DirectX::SimpleMath::Vector2(0.7f, 0.7f));
	showTimeSprite->setShow(false);

	auto clickableRetrySprite = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-rect.jpg", buttonsX, 340, 0.6f, 0.4f, 0.f, 0.5f,
		[this]() { m_resultMenu->retryLevel(); });

	graphics->addToSpriteBatch(clickableRetrySprite);
	clickableRetrySprite->setShow(false);
	auto retryTextSprite = graphics->getResourceDevice()->createSprite("Retry Level", L"Textures/Sprites/Fonts/font.spritefont", buttonsX + 115, 385);
	graphics->addToSpriteBatch(retryTextSprite);
	retryTextSprite->setScale(DirectX::SimpleMath::Vector2(0.5, 0.5));
	retryTextSprite->setShow(false);

	auto clickableNextSprite = graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/InGameMenu/BlankPanel-rect.jpg", buttonsX, 460, 0.6f, 0.4f, 0.f, 0.5f,
		[this]() { m_resultMenu->nextLevel(); });

	graphics->addToSpriteBatch(clickableNextSprite);
	clickableNextSprite->setShow(false);
	auto nextTextSprite = graphics->getResourceDevice()->createSprite("Next Level", L"Textures/Sprites/Fonts/font.spritefont", buttonsX + 115, 505);
	graphics->addToSpriteBatch(nextTextSprite);
	nextTextSprite->setScale(DirectX::SimpleMath::Vector2(0.5, 0.5));
	nextTextSprite->setShow(false);

	std::vector<std::shared_ptr<ISprite>> sprites;
	sprites.push_back(clickableSprite);
	sprites.push_back(clickableSprite2);
	sprites.push_back(clickableSprite3);
	sprites.push_back(menuBack);
	sprites.push_back(text1);
	sprites.push_back(text2);
	sprites.push_back(text3);
	m_menu = std::make_shared<InGameMenu>(sprites);

	std::vector<std::shared_ptr<ISprite>> resultSprites;
	resultSprites.push_back(showGradeSprite);
	resultSprites.push_back(clickableRetrySprite);
	resultSprites.push_back(retryTextSprite);
	resultSprites.push_back(nextTextSprite);
	resultSprites.push_back(clickableNextSprite);
	resultSprites.push_back(showTimeSprite);

	m_resultMenu = std::make_shared<ResultMenu>(resultSprites, showGradeSprite, showTimeSprite, this);

	addObserver(m_resultMenu);
}

void SceneManager::createScenes(Graphics* graphics)
{
	/*Ref<Scene> mainMenu = std::make_shared<MainMenu>(graphics, this);
	addScene(mainMenu);*/
	Ref<Scene> scene = std::make_shared<Scene1>(graphics);
	addScene(scene);
	Ref<Scene> scene2 = std::make_shared<Scene2>(graphics);
	addScene(scene2);
	Ref<Scene> scene3 = std::make_shared<Scene3>(graphics);
	addScene(scene3);
	Ref<Scene> scene4 = std::make_shared<Scene4>(graphics);
	addScene(scene4);
	Ref<Scene> scene5 = std::make_shared<Scene5>(graphics);
	addScene(scene5);
	
	setActiveScene(scene5);
	//setActiveScene(scene2);
}

void SceneManager::setActiveScene(Ref<Scene> newActiveScene)
{
	m_activeScene = newActiveScene;
	m_activeScene->setUpScene();
	//Call start on all GameObjects in Scene
	m_activeScene->start();

	updateObservers();
}

void SceneManager::addScene(Ref<Scene> newScene)
{
	newScene->setMenu(m_menu, m_resultMenu);
	m_scenes.push_back(newScene);
}

Ref<Scene> SceneManager::getActiveScene() const
{
	return m_activeScene;
}

void SceneManager::changeScene(const int& sceneIndex)
{
	if (sceneIndex > m_scenes.size() - 1)
		assert(false);

	Ref<Scene> newActiveScene = m_scenes[sceneIndex];
	m_activeScene->emptyScene();

	setActiveScene(newActiveScene);
}

void SceneManager::updateActiveScene() const
{
	m_activeScene->update();
}

void SceneManager::addObserver(Ref<SceneManagerObserver> observer)
{
	m_observers.push_back(observer);
	observer->updateScene(m_activeScene);
}

void SceneManager::removeObserver(Ref<SceneManagerObserver> observer)
{
	for (int i = 0; i < m_observers.size(); ++i)
	{
		if (observer == m_observers[i])
		{
			m_observers.erase(m_observers.begin() + i);
			break;
		}
	}
}

bool SceneManager::currentScenePaused() const
{
	return m_activeScene->isPaused();
}

void SceneManager::quitGame()
{
	m_timeToQuit = true;
}

bool SceneManager::shouldQuit() const
{
	return m_timeToQuit;
}

void SceneManager::updateObservers()
{
	for (int i = 0; i < m_observers.size(); ++i)
	{
		m_observers[i]->updateScene(m_activeScene);
	}
}