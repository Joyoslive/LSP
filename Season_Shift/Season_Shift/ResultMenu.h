#pragma once

class ISprite;

class ResultMenu
{
private:
	std::vector<std::shared_ptr<ISprite>> m_sprites;
	std::shared_ptr<ISprite> m_text;

public:
	ResultMenu(std::vector<std::shared_ptr<ISprite>> sprites, std::shared_ptr<ISprite> textSprite);
	~ResultMenu();
	void setShowSprites(bool show);

	void playerReachedGoal(long double time, const std::string& grade);
};
