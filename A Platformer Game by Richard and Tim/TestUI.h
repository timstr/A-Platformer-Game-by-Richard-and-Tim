#pragma once

#include "gui.h"
#include "TestSpace.h"

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		if (play){
			float speed = 0.0f;
			float jump = 0.0f;
			if (keyDown(sf::Keyboard::Left)){
				speed -= 1;
			}
			if (keyDown(sf::Keyboard::Right)){
				speed += 1;
			}
			if (keyDown(sf::Keyboard::Up)){
				jump += 10;
			}
			space.guy->updateMoves(speed, jump);

			if (leftMouseDown() && (keyDown(sf::Keyboard::LShift) || keyDown(sf::Keyboard::RShift))){
				for (Entity* const entity : space.entities){
					vec2 disp = ui::getMousePos() - entity->position;
					double dist = abs(disp);
					if (dist > 0.0){
						entity->velocity += disp * (float)(50.0 / pow(dist, 2));
					}
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

	void onLeftClick(int clicks) override {
		//if (keyDown(sf::Keyboard::LControl) || keyDown(sf::Keyboard::RControl)){
			space.createEntity(localMousePos());
		//}
	}

	TestSpace space;
	bool play = true;
};