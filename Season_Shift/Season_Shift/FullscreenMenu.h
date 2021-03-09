#pragma once
#include "Graphics/DX/DXTexture.h"

class FullscreenMenu
{
private:
	std::string m_title;
	std::shared_ptr<DXTexture> m_background;
	//std::vector<Button> buttons
private:
	void setupMenu();
public:
	FullscreenMenu(const std::string& title, const std::string& bgPath);
	~FullscreenMenu() = default;

	//void addButton();
	void render();
};

