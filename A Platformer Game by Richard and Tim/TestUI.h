#pragma once

#include "gui.h"
#include "TestSpace.h"

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		space.probe = ui::getMousePos();
		space.tick();
		space.render(rw, offset);
	}

	Platformer::TestSpace space;
};