#pragma once

#include <gui/gui.h>
#include "TestSpace.h"

sf::Font& getFont();

struct TestSpaceWindow : ui::FreeElement {
	TestSpaceWindow();

	void render(sf::RenderWindow& rw) override;

	bool onKeyDown(sf::Keyboard::Key key) override;

	bool onLeftClick(int clicks) override;

private:
	ui::Ref<ui::Text> label;
	TestSpace space;
	bool play = true;

	const float scale_step = 1.05f;
	sf::Transform viewtransform;
};