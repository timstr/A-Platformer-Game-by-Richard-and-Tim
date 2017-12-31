#pragma once

#include "gui.h"
#include "TestSpace.h"

sf::Font& getFont(){
	static sf::Font font;
	static bool loaded;
	if (!loaded){
		font.loadFromFile("fonts/JosefinSans-Bold.ttf");
		loaded = true;
	}
	return font;
}

struct TestSpaceWindow : ui::Window {
	TestSpaceWindow(){
		size = {1000, 700};
		addChildWindow(label = new ui::Text("-", getFont()));
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		 if (keyDown(sf::Keyboard::Equal)){
			 vec2 center = viewtransform.getInverse().transformPoint(size * 0.5f);
			 viewtransform.scale(vec2(scale_step, scale_step), center);
		 }
		 if (keyDown(sf::Keyboard::Dash)){
			 vec2 center = viewtransform.getInverse().transformPoint(size * 0.5f);
			 viewtransform.scale(vec2(1.0f / scale_step, 1.0f / scale_step), center);
		 }
		 if (keyDown(sf::Keyboard::A)){
			 sf::Transform t;
			 t.translate(vec2(5, 0));
			 viewtransform = t * viewtransform;
		 }
		 if (keyDown(sf::Keyboard::D)){
			 sf::Transform t;
			 t.translate(vec2(-5, 0));
			 viewtransform = t * viewtransform;
		 }
		 if (keyDown(sf::Keyboard::W)){
			 sf::Transform t;
			 t.translate(vec2(0, 5));
			 viewtransform = t * viewtransform;
		 }
		 if (keyDown(sf::Keyboard::S)){
			 sf::Transform t;
			 t.translate(vec2(0, -5));
			 viewtransform = t * viewtransform;
		 }

		if (play){
			if (keyDown(sf::Keyboard::Left)){
				space.guy->runLeft();
			}
			if (keyDown(sf::Keyboard::Right)){
				space.guy->runRight();
			}
			if (keyDown(sf::Keyboard::Up)){
				space.guy->jump();
			}
			if (keyDown(sf::Keyboard::Return)){
				space.guy->attack();
			}

			if (leftMouseDown() && (keyDown(sf::Keyboard::LShift) || keyDown(sf::Keyboard::RShift))){
				for (Entity* const entity : space.entities){
					vec2 p = viewtransform.getInverse().transformPoint(localMousePos());
					vec2 disp = p - entity->getPosition();
					double dist = abs(disp);
					if (dist > 0.0){
						entity->velocity += disp * (float)(50.0 / pow(dist, 2));
					}
				}
			}

			space.tick();
		}


		sf::Transform translation;
		translation.translate(offset);
		translation *= viewtransform;
		rw.draw(space, translation);

		label->setText(std::to_string(space.entities.size()) + " Entities");

		renderChildWindows(rw, offset);
	}

	void onKeyDown(sf::Keyboard::Key key) override {
		if (key == sf::Keyboard::Space){
			play = !play;
		}
	}

	void onLeftClick(int clicks) override {
		if (keyDown(sf::Keyboard::LControl) || keyDown(sf::Keyboard::RControl)){
			vec2 p = localMousePos();
			space.createEntity(viewtransform.getInverse().transformPoint(p));
		}
	}

	private:
	ui::Text* label;
	TestSpace space;
	bool play = true;

	const float scale_step = 1.05f;
	sf::Transform viewtransform;
};