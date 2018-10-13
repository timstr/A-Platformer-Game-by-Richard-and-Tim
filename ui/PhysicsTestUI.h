#pragma once

#include "gui/gui.h"
#include "physicstest.h"
#include "TestUI.h"
#include <random>

std::random_device rand_dev;
std::default_random_engine rand_eng {rand_dev()};

struct PhysicsTestUI : ui::FreeElement {
	PhysicsTestUI(vec2 _size) : paused(true) {
		setSize(_size, true);

		int n = 100;
		std::uniform_real_distribution<float> dist {0.0f, 1.0f};

		for (int i = 0; i < n; ++i){
			auto shape = std::make_unique<CircleGuy>(
				2.0f + 18.0f * dist(rand_eng),
				vec2(_size.x * dist(rand_eng), _size.y * dist(rand_eng))
			);
			vec2 velo = shape->body.getPosition() - _size * 0.5f;
			velo *= -2.0f / abs(velo);
			shape->body.setVelocity(velo);

			physics_engine.addRigidBody(shape->body);

			shapes.emplace_back(std::move(shape));
		}
	}
	~PhysicsTestUI() {
		for (const auto& shape : shapes) {
			physics_engine.removeRigidBody(shape->getBody());
		}
		shapes.clear();
	}

	bool onKeyDown(ui::Key key) override {
		if (key == ui::Key::Space) {
			paused = !paused;
		} else if (key == ui::Key::T && paused) {
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
	}

private:

	ui::Ref<ui::Text> label;
	bool paused;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};