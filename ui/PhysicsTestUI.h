#pragma once

#include "gui/gui.h"
#include "physicstest.h"
#include "TestUI.h"
#include <random>

struct PhysicsTestUI : ui::FreeElement {
	PhysicsTestUI(vec2 _size) : paused(true) {
		setSize(_size, true);

		shapes.emplace_back(std::make_unique<CircleGuy>(50.0f, vec2(700.0f, 100.0f)));
		shapes.emplace_back(std::make_unique<CircleGuy>(50.0f, vec2(700.0f, 300.0f)));

		physics_engine.addRigidBody(shapes[0]->getBody());
		physics_engine.addRigidBody(shapes[1]->getBody());

		shapes[0]->getBody().applyForce({ 0.0f, 0.0f });
		shapes[1]->getBody().applyForce({ -300000.0f, 0.0f });

		label = add<ui::Text>("", getFont());
	}
	~PhysicsTestUI() {
		for (const auto& shape : shapes) {
			physics_engine.removeRigidBody(shape->getBody());
		}
		shapes.clear();
	}

	bool onKeyDown(sf::Keyboard::Key key) override {
		if (key == sf::Keyboard::Space) {
			paused = !paused;
		} else if (key == sf::Keyboard::T && paused) {
			physics_engine.tick(1.0f);
		}
		return true;
	}

	void render(sf::RenderWindow& rw) override {
		if (!paused) {
			physics_engine.tick(1.0f);
		}

		sf::RectangleShape rect;
		rect.setSize(getSize());
		rect.setFillColor(sf::Color(0xFFFFFFFF));
		rect.setOutlineColor(sf::Color(0xFF));
		rect.setOutlineThickness(1.0f);

		rw.draw(rect);

		for (const auto& shape : shapes) {
			shape->draw(rw);
		}

		float dist = abs(shapes[1]->getBody().getPosition() - shapes[0]->getBody().getPosition());
		label->setText("Distance: " + std::to_string(dist));
	}

private:

	ui::Ref<ui::Text> label;
	bool paused;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};