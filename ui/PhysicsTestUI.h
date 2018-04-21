#pragma once

#include "gui/gui.h"
#include "physicstest.h"
#include "TestUI.h"
#include <random>

struct PhysicsTestUI : ui::Window {
	PhysicsTestUI(vec2 _size, size_t num_bodies) : paused(true) {
		size = _size;

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<int> idist {0, 1};
		std::uniform_real_distribution<float> fdist {0.0f, 1.0f};

		shapes.reserve(num_bodies);

		for (size_t i = 0; i < num_bodies; i++){
			vec2 pos {
				fdist(eng) * size.x,
				fdist(eng) * size.y
			};

			if (idist(eng)){
				float rad = 5.0f + fdist(eng) * 45.0f;
				shapes.emplace_back(std::make_unique<CircleGuy>(rad, pos));
			} else {
				vec2 size {
					10.0f + fdist(eng) * 90.0f,
					10.0f + fdist(eng) * 90.0f
				};
				shapes.emplace_back(std::make_unique<RectangleGuy>(size, pos));
			}

			vec2 velocity {
				(fdist(eng) * 2.0f - 1.0f) * 2.0f,
				(fdist(eng) * 2.0f - 1.0f) * 2.0f
			};
			float angular_velocity = (fdist(eng) * 2.0f - 1.0f) * 1.0f;

			phys::RigidBody& body = shapes.back()->getBody();

			body.setVelocity(velocity);
			body.setAngularVelocity(angular_velocity);

			physics_engine.addRigidBody(shapes.back()->getBody());
		}
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
	}

	private:

	bool paused;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};