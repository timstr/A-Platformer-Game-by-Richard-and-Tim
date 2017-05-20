#pragma once

#include "gui.h"
#include "TestSpace.h"

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		space.probe = ui::getMousePos();

		vec2 dir = {0, 0};
		if (keyDown(sf::Keyboard::Left)){
			dir.x -= 1;
		}
		if (keyDown(sf::Keyboard::Right)){
			dir.x += 1;
		}
		if (keyDown(sf::Keyboard::Up)){
			dir.y -= 1;
		}
		if (keyDown(sf::Keyboard::Down)){
			dir.y += 1;
		}
		space.guy->move(dir);

		space.tick();
		space.render(rw, offset);
	}

	TestSpace space;
};