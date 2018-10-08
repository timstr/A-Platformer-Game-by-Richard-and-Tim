#include "TestUI.h"
#include "fontspath.h"
#include "SFML/Graphics.hpp"

sf::Font& getFont() {
	static sf::Font font;
	static bool loaded;
	if (!loaded) {
		font.loadFromFile(fonts_path + "/JosefinSans-Bold.ttf");
		loaded = true;
	}
	return font;
}

TestSpaceWindow::TestSpaceWindow() {
	setBackgroundColor(sf::Color(0xFFFFFFFF));
	label = add<ui::Text>("-", getFont());
}

void TestSpaceWindow::render(sf::RenderWindow& rw) {
	setSize(ui::getScreenSize(), true);

	// TODO: Make the view transform track the player

	vec2 center = viewtransform.getInverse().transformPoint(getSize() * 0.5f);
	vec2 diff = center - space.guy->getPosition();
	viewtransform.translate(diff / 20.0f);

	if (keyDown(ui::Key::Equal)) {
		viewtransform.scale(vec2(scale_step, scale_step), center);
	}
	if (keyDown(ui::Key::Dash)) {
		viewtransform.scale(vec2(1.0f / scale_step, 1.0f / scale_step), center);
	}
	if (keyDown(ui::Key::A)) {
		sf::Transform t;
		t.translate(vec2(5, 0));
		viewtransform = t * viewtransform;
	}
	if (keyDown(ui::Key::D)) {
		sf::Transform t;
		t.translate(vec2(-5, 0));
		viewtransform = t * viewtransform;
	}
	if (keyDown(ui::Key::W)) {
		sf::Transform t;
		t.translate(vec2(0, 5));
		viewtransform = t * viewtransform;
	}
	if (keyDown(ui::Key::S)) {
		sf::Transform t;
		t.translate(vec2(0, -5));
		viewtransform = t * viewtransform;
	}

	if (play) {
			if (keyDown(ui::Key::Left)) {
				space.guy->runLeft();
			}
			if (keyDown(ui::Key::Right)) {
				space.guy->runRight();
			}
			if (keyDown(ui::Key::Up)) {
				space.guy->jump();
			}

		// gravitation mouse fling
		if (leftMouseDown() && (keyDown(ui::Key::LShift) || keyDown(ui::Key::RShift))) {
			for (const auto& ent : space.entities) {
				if (auto entity = ent.lock()) {
					vec2 p = viewtransform.getInverse().transformPoint(localMousePos());
					vec2 disp = p - entity->getPosition();
					float dist = abs(disp);
					if (dist > 0.0) {
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
}

bool TestSpaceWindow::onKeyDown(ui::Key key) {
	vec2 p = viewtransform.getInverse().transformPoint(localMousePos());
	switch (key){
	case ui::Key::Space:
		play = !play;
		break;
	case ui::Key::Num1:
		space.addCreatures<TestWorm>(1, p);
		break;
	case ui::Key::Num2:
		space.addCreatures<TestBird>(1, p);
		break;
	case ui::Key::Num3:
		space.addCreatures<TestBug>(1, p);
		break;
	case ui::Key::Num4:
		space.addCreatures<TestCharacter>(1, p);
		break;
	case ui::Key::Num5:
		space.addCreatures<HermannHorst>(1, p);
		break;
	} 
	return true;
}

bool TestSpaceWindow::onLeftClick(int clicks) {
	if (keyDown(ui::Key::LControl) || keyDown(ui::Key::RControl)) {
		vec2 p = localMousePos();
		space.createEntity(viewtransform.getInverse().transformPoint(p));
	}
	return true;
}