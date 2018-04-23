#pragma once

#include "gui/gui.h"
#include "physicstest.h"
#include "TestUI.h"
#include <random>

struct PhysicsTestUI : ui::Window {
	PhysicsTestUI(vec2 _size) : paused(true) {
		size = _size;

		shapes.emplace_back(std::make_unique<CircleGuy>(50.0f, size * 0.5f - vec2(200.0f, 0.0f)));
		shapes.emplace_back(std::make_unique<CircleGuy>(50.0f, size * 0.5f + vec2(200.0f, 0.0f)));

		physics_engine.addRigidBody(shapes[0]->getBody());
		physics_engine.addRigidBody(shapes[1]->getBody());

		shapes[0]->getBody().applyForce({0.0f, -30000.0f});
		shapes[1]->getBody().applyForce({0.0f, 30000.0f});

		addChildWindow(label = new ui::Text("", getFont()), vec2(0.0f, 0.0f));
	}
	~PhysicsTestUI(){
		for (const auto& shape : shapes){
			physics_engine.removeRigidBody(shape->getBody());
		}
		shapes.clear();
	}

	void onKeyDown(sf::Keyboard::Key key) override {
		if (key == sf::Keyboard::Space){
			paused = !paused;
		}
	}

	void render(sf::RenderWindow& rw) override {
		if (!paused){
			physics_engine.tick(1.0f);
		}

		sf::RectangleShape rect;
		rect.setSize(size);
		rect.setFillColor(sf::Color(0xFFFFFFFF));
		rect.setOutlineColor(sf::Color(0xFF));
		rect.setOutlineThickness(1.0f);

		rw.draw(rect);

		for (const auto& shape : shapes){
			shape->draw(rw);
		}

		float dist = abs(shapes[1]->getBody().getPosition() - shapes[0]->getBody().getPosition());
		label->setText("Distance: " + std::to_string(dist));

		renderChildWindows(rw);
	}

	private:

	ui::Text* label;
	bool paused;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};