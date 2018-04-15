#include "TestUI.h"
#include "fontspath.h"

sf::Font& getFont(){
	static sf::Font font;
	static bool loaded;
	if (!loaded){
		font.loadFromFile(fonts_path + "/JosefinSans-Bold.ttf");
		loaded = true;
	}
	return font;
}

TestSpaceWindow::TestSpaceWindow(){
	size = {1000, 700};
	addChildWindow(label = new ui::Text("-", getFont()));
}

void TestSpaceWindow::render(sf::RenderWindow& rw){
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
		if (auto sguy = space.guy.lock()){
			if (keyDown(sf::Keyboard::Left)){
				sguy->runLeft();
			}
			if (keyDown(sf::Keyboard::Right)){
				sguy->runRight();
			}
			if (keyDown(sf::Keyboard::Up)){
				sguy->jump();
			}
			if (keyDown(sf::Keyboard::Return)){
				sguy->attack();
			}
		}

		// gravitation mouse fling
		if (leftMouseDown() && (keyDown(sf::Keyboard::LShift) || keyDown(sf::Keyboard::RShift))){
			for (const auto& ent : space.entities){
				if (auto entity = ent.lock()){
					vec2 p = viewtransform.getInverse().transformPoint(localMousePos());
					vec2 disp = p - entity->getPosition();
					float dist = abs(disp);
					if (dist > 0.0){
						entity->velocity += disp * (float)(50.0 / pow(dist, 2));
					}
				}
			}
		}

		space.tick();
	}


	sf::Transform translation;
	translation *= viewtransform;
	rw.draw(space, translation);

	label->setText(std::to_string(space.entities.size()) + " Entities");

	renderChildWindows(rw);
}

void TestSpaceWindow::onKeyDown(sf::Keyboard::Key key){
	if (key == sf::Keyboard::Space){
		play = !play;
	}
}

void TestSpaceWindow::onLeftClick(int clicks){
	if (keyDown(sf::Keyboard::LControl) || keyDown(sf::Keyboard::RControl)){
		vec2 p = localMousePos();
		space.createEntity(viewtransform.getInverse().transformPoint(p));
	}
}