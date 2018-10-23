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

		// two test circles
		/*auto s1 = std::make_unique<CircleGuy>(25.0f, _size * 0.5f - vec2{100.0f, 0.0f});
		auto s2 = std::make_unique<CircleGuy>(25.0f, _size * 0.5f + vec2{100.0f, 0.0f});
		s1->body.setVelocity({1.0f, 0.0f});
		s2->body.setVelocity({-1.0f, 0.0f});
		physics_engine.addRigidBody(s1->body);
		physics_engine.addRigidBody(s2->body);
		shapes.emplace_back(std::move(s1));
		shapes.emplace_back(std::move(s2));*/

		// two test rectangles
		/*auto s1 = std::make_unique<RectangleGuy>(vec2{300.0f, 25.0f}, _size * 0.5f - vec2{100.0f, 0.0f});
		auto s2 = std::make_unique<RectangleGuy>(vec2{300.0f, 25.0f}, _size * 0.5f + vec2{100.0f, 0.0f});
		s1->body.setVelocity({0.5f, 0.0f});
		s2->body.setVelocity({-0.5f, 0.0f});
		s1->body.setAngle(-0.2f);
		s2->body.setAngle(-0.2f);
		s1->body.setAngularVelocity(0.01f);
		s2->body.setAngularVelocity(0.01f);
		physics_engine.addRigidBody(s1->body);
		physics_engine.addRigidBody(s2->body);
		shapes.emplace_back(std::move(s1));
		shapes.emplace_back(std::move(s2));*/

		// many random shapes
		int n = 100;
		std::uniform_real_distribution<float> dist {0.0f, 1.0f};
		std::uniform_int_distribution<int> bdist {0, 1};

		for (int i = 0; i < n; ++i){
			std::unique_ptr<ShapeGuy> shape;
			/*if (bdist(rand_eng)){
				shape = std::make_unique<CircleGuy>(
					2.0f + 18.0f * dist(rand_eng),
					vec2(_size.x * dist(rand_eng), _size.y * dist(rand_eng))
				);
			} else {*/
				shape = std::make_unique<RectangleGuy>(
					vec2(5.0f + dist(rand_eng) * 45.0f, 5.0f + dist(rand_eng) * 45.0f),
					vec2(_size.x * dist(rand_eng), _size.y * dist(rand_eng))
				);
			//}
			vec2 velo = shape->getBody().getPosition() - _size * 0.5f;
			velo *= -1.0f / abs(velo);
			shape->getBody().setVelocity(velo);
			shape->getBody().setAngle(dist(rand_eng) * 2.0f * pi);

			physics_engine.addRigidBody(shape->getBody());

			shapes.emplace_back(std::move(shape));
		}

		// cannonball
		/*auto cannonball = std::make_unique<CircleGuy>(50.0f, vec2{-_size.x * 3.0f, _size.y * 0.5f});
		cannonball->getBody().setVelocity({10.0f, 0.0f});
		physics_engine.addRigidBody(cannonball->getBody());
		shapes.emplace_back(std::move(cannonball));*/

		// cannonbox
		auto cannonbox = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{-_size.x * 3.0f, _size.y * 0.5f});
		cannonbox->getBody().setVelocity({10.0f, 0.0f});
		cannonbox->getBody().setAngle(pi * 0.25f);
		physics_engine.addRigidBody(cannonbox->getBody());
		shapes.emplace_back(std::move(cannonbox));
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

		sf::CircleShape s;
		s.setOutlineThickness(2.0f);
		s.setFillColor(sf::Color(0));
		s.setOrigin({2.5f, 2.5f});
		s.setRadius(2.5f);
		s.setOutlineColor(sf::Color(0xFF));

		for (const auto& shape : shapes) {
			shape->draw(rw);
			s.setPosition(shape->getBody().getPosition());
			rw.draw(s);
		}

		s.setRadius(5.0f);
		s.setOrigin({5.0f, 5.0f});
		s.setOutlineColor(sf::Color(0xFF0000FF));

		for (const auto& collision : physics_engine.getCollisions()){
			s.setPosition(collision.radius_a + collision.a.getPosition());
			rw.draw(s);

			sf::Vertex vertices[2] = {
				{{collision.a.getPosition() + collision.radius_a}, sf::Color(0xFF)},
				{{collision.a.getPosition() + collision.radius_a + 20.0f * collision.normal}, sf::Color(0xFF)}
			};

			rw.draw(vertices, 2, sf::LineStrip);
		}
	}

private:

	ui::Ref<ui::Text> label;
	bool paused;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};