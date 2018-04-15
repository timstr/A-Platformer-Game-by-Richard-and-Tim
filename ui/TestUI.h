#pragma once

#include <gui\gui.h>
#include "TestSpace.h"

sf::Font& getFont();

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow();

	void render(sf::RenderWindow& rw) override;

	void onKeyDown(sf::Keyboard::Key key) override;

	void onLeftClick(int clicks) override;

	private:
	ui::Text* label;
	TestSpace space;
	bool play = true;

	const float scale_step = 1.05f;
	sf::Transform viewtransform;
};