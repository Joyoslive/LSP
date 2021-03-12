#pragma once
#include "SceneManagerObserver.h"

class ISprite;
class SceneManager;

class ResultMenu : public SceneManagerObserver
{
private:
	std::vector<std::shared_ptr<ISprite>> m_sprites;
	std::shared_ptr<ISprite> m_gradeText;
	std::shared_ptr<ISprite> m_timeText;
	Scene* m_scene;
	SceneManager* m_sceneManager;
	bool m_active;

public:
	ResultMenu(std::vector<std::shared_ptr<ISprite>> sprites, std::shared_ptr<ISprite> gradeSprite, std::shared_ptr<ISprite> timeSprite, SceneManager* sceneManager);
	~ResultMenu();
	void setShowSprites(bool show);

	void playerReachedGoal(long double time, const std::string& grade);
	void retryLevel();
	void nextLevel();


	void updateScene(Scene* activeScene);
	bool getActive() const;
};
