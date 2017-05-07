#pragma once

#include "gui.h"
#include "TestSpace.h"

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		space.probe = ui::getMousePos();
		for (Entity* entity : space.entities){
			entity->velocity.y += 0.3;
			//entity->velocity *= 0.99f;
		}
		space.tick();
		space.render(rw, offset);
	}

	TestSpace space;
};