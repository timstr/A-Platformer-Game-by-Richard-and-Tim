#pragma once

#include "gui.h"
#include "TestSpace.h"

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		space.probe = ui::getMousePos();

		if (play){
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

			if (leftMouseDown()){
				for (Entity* const entity : space.entities){
					vec2 disp = ui::getMousePos() - entity->position;
					entity->velocity += disp * (float)(50.0 / pow(abs(disp), 2));
				}
			}

			space.tick();
		}

		space.render(rw, offset);
	}

	void onKeyDown(sf::Keyboard::Key key) override {
		if (key == sf::Keyboard::Space){
			play = !play;
		}
	}

	TestSpace space;
	bool play = true;
};