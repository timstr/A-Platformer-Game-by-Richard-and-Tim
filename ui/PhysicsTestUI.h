#pragma once

#include "gui/gui.h"
#include "physicstest.h"
#include "TestUI.h"
#include <random>

std::random_device rand_dev;
std::default_random_engine rand_eng {rand_dev()};

struct PhysicsTestUI : ui::FreeElement {
	PhysicsTestUI(vec2 _size) :
		paused(true),
		gravity_dir(0),
		recording(false),
		screenshot_counter(0),
		show_corners(true),
		show_contacts(true) {

		setSize(_size, true);

		// friction test
		/*{
			auto s1 = std::make_unique<RectangleGuy>(vec2{200.0f, 100.0f}, vec2{150.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			//auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{300.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{150.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngularVelocity(0.11f);
			s1->body.setAngle(pi * -0.125f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{900.0f, 10.0f}, vec2{500.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{500.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			//auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{500.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s1->body.setAngle(pi * 0.125f);
			s2->body.setFriction(0.0f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{200.0f, 100.0f}, vec2{850.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{850.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s1->body.setAngle(pi * 0.125f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}*/

		// penetration test
		/*{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{350.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{350.0f, 250.0f - 100.0f / sqrt(2.0f)}, sf::Color(0xC0C0C0FF));
			//auto s2 = std::make_unique<CircleGuy>(50.0f, vec2{350.0f, 210.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<CircleGuy>(50.0f, vec2{500.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<CircleGuy>(50.0f, vec2{500.0f, 201.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}*/




		// rectangle-rectangle test suite
		/*{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{75.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{75.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{200.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{200.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{300.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{60.0f, 100.0f}, vec2{300.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{400.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{400.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{500.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{40.0f, 100.0f}, vec2{500.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{600.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{25.0f, 100.0f}, vec2{600.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{700.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{675.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{825.0f, 300.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{850.0f, 100.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		/////////////////////////////////////
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{75.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{75.0f, 410.0f}, sf::Color(0x808080FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{200.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{200.0f, 410.0f}, sf::Color(0x808080FF));
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{300.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{60.0f, 100.0f}, vec2{300.0f, 410.0f}, sf::Color(0x808080FF));
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{400.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{400.0f, 410.0f}, sf::Color(0x808080FF));
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{500.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{40.0f, 100.0f}, vec2{500.0f, 410.0f}, sf::Color(0x808080FF));
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{50.0f, 100.0f}, vec2{600.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{25.0f, 100.0f}, vec2{600.0f, 410.0f}, sf::Color(0x808080FF));
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{700.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{675.0f, 410.0f}, sf::Color(0x808080FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s2));
			addBody(std::move(s1));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{825.0f, 610.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{50.0f, 25.0f}, vec2{850.0f, 410.0f}, sf::Color(0x808080FF));
			s2->body.setAngle(pi * 0.25f);
			addBody(std::move(s2));
			addBody(std::move(s1));
		}*/

		// chain test
		{

			float pos = 200.0f;
			std::unique_ptr<RectangleGuy> prev = std::make_unique<RectangleGuy>(vec2{20.0f, 10.0f}, vec2{pos, 300.0f}, sf::Color(0x8117D1FF));
			std::unique_ptr<RectangleGuy> next;
			for (int i = 0; i < 29; ++i){
				pos += 25.0f;
				next = std::make_unique<RectangleGuy>(vec2{20.0f, 10.0f}, vec2{pos, 300.0f}, sf::Color(0x8117D1FF));
				physics_engine.addConstraint(std::make_unique<phys::HingeConstraint>(prev->getBody(), next->getBody(), vec2{10.0f, 0.0f}, vec2{-10.0f, 0.0f}, 5.0f));
				addBody(std::move(prev));
				prev = std::move(next);
			}
			auto ball = std::make_unique<CircleGuy>(20.0f, vec2{pos + 35.0f, 300.0f}, sf::Color(0x712378FF));
			physics_engine.addConstraint(std::make_unique<phys::HingeConstraint>(prev->getBody(), ball->getBody(), vec2{10.0f, 0.0f}, vec2{-20.0f, 0.0f}, 5.0f));
			addBody(std::move(ball));
			addBody(std::move(prev));

			//auto s3 = std::make_unique<RectangleGuy>(vec2{300.0f, 50.0f}, vec2{650.0f, 500.0f}, sf::Color(0xC0C0C0FF), 0.0f);
			//s3->body.setAngle(pi * 0.125f);
			auto s3 = std::make_unique<CircleGuy>(100.0f, vec2{650.0f, 450.0f}, sf::Color(0x604040FF), 0.0f);
			addBody(std::move(s3));
		}

		// many random shapes
        /*{
            int n = 40;
			const float min_rad = 30.0f;
			const float max_rad = 50.0f;
			const int cols = 8;
			const float padding = 10.0f;
			const float spacing = (_size.x - 2.0f * padding) / (float)cols;

            std::uniform_real_distribution<float> dist {0.0f, 1.0f};
			std::uniform_int_distribution<int> bdist {0, 1};
			std::uniform_int_distribution<int> idist {0, 255};

            for (int i = 0; i < n; ++i){
                std::unique_ptr<ShapeGuy> shape;
                vec2 pos = {
                    ((float)(i % cols) + 0.5f) * spacing + padding + dist(rand_eng) * 1.0f,
                    _size.y - ((float)(i / cols) + 0.5f) * spacing - padding - dist(rand_eng) * 1.0f
                };

				auto color = sf::Color(idist(rand_eng), idist(rand_eng), idist(rand_eng), 255);

                if (bdist(rand_eng)){
				    shape = std::make_unique<CircleGuy>(min_rad + dist(rand_eng) * (max_rad - min_rad), pos, color);
                } else {
				    shape = std::make_unique<RectangleGuy>(vec2(2.0f * (min_rad + dist(rand_eng) * (max_rad - min_rad)), 2.0f * (min_rad + dist(rand_eng) * (max_rad - min_rad))), pos, color);
                }

                //vec2 velo = shape->getBody().getPosition() - _size * 0.5f;
                //velo *= -0.5f / abs(velo);
                //shape->getBody().setVelocity(velo);

                //shape->getBody().setAngle(dist(rand_eng) * 2.0f * pi);
                //shape->getBody().setAngularVelocity((-1.0f + 2.0f * dist(rand_eng)) * 0.1f);

                addBody(std::move(shape));
            }
        }*/

		// cannonball
		/*auto cannonball = std::make_unique<CircleGuy>(250.0f, vec2{-_size.x * 3.0f, _size.y * 0.5f});
		cannonball->getBody().setVelocity({20.0f, 0.0f});
		physics_engine.addRigidBody(cannonball->getBody());
		shapes.emplace_back(std::move(cannonball));*/

		// cannonbox
		/*auto cannonbox = std::make_unique<RectangleGuy>(vec2{500.0f, 500.0f}, vec2{_size.x * 4.0f, _size.y * 0.5f});
		cannonbox->getBody().setVelocity({-20.0f, 0.0f});
		cannonbox->getBody().setAngle(pi * 0.25f);
		physics_engine.addRigidBody(cannonbox->getBody());
		shapes.emplace_back(std::move(cannonbox));*/

		// border boxes
		auto topwall = std::make_unique<RectangleGuy>(vec2{_size.x, 10.0f}, vec2{_size.x * 0.5f, 5.0f}, sf::Color(0x804040FF), 0.0f);
		auto bottomwall = std::make_unique<RectangleGuy>(vec2{_size.x, 10.0f}, vec2{_size.x * 0.5f, _size.y - 5.0f}, sf::Color(0x804040FF), 0.0f);
		auto leftwall = std::make_unique<RectangleGuy>(vec2{10.0f, _size.y - 20.0f}, vec2{5.0f, _size.y * 0.5f}, sf::Color(0x804040FF), 0.0f);
		auto rightwall = std::make_unique<RectangleGuy>(vec2{10.0f, _size.y - 20.0f}, vec2{_size.x - 5.0f, _size.y * 0.5f}, sf::Color(0x804040FF), 0.0f);
		physics_engine.addRigidBody(&topwall->getBody());
		physics_engine.addRigidBody(&bottomwall->getBody());
		physics_engine.addRigidBody(&leftwall->getBody());
		physics_engine.addRigidBody(&rightwall->getBody());
		shapes.emplace_back(std::move(topwall));
		shapes.emplace_back(std::move(bottomwall));
		shapes.emplace_back(std::move(leftwall));
		shapes.emplace_back(std::move(rightwall));
	}
	~PhysicsTestUI() {
		for (const auto& shape : shapes) {
			physics_engine.removeRigidBody(&shape->getBody());
		}
		shapes.clear();
	}

	void addBody(std::unique_ptr<ShapeGuy> shape){
		physics_engine.addRigidBody(&shape->getBody());
		shapes.emplace_back(std::move(shape));
	}

	void tick(){

        // slow motion if shift is held
        float delta = 2.0f * (keyDown(ui::Key::LShift) || keyDown(ui::Key::RShift)) ? 0.1f : 1.0f;

        const vec2 directions[] = {
            {0.0f, 1.0f},
            {-1.0f, 0.0f},
            {0.0f, -1.0f},
            {1.0f, 0.0f}
        };

		// apply gravity
		for (const auto& shape : shapes){
			auto& body = shape->getBody();
			body.applyForce(directions[gravity_dir] * 0.01f * body.mass, delta);
		}
		
		// simulate
		physics_engine.tick(delta);

        if (recording){
            sf::Texture tex;
            tex.create(ui::getScreenSize().x, ui::getScreenSize().y);
            tex.update(ui::getContext().getRenderWindow());
            tex.copyToImage().saveToFile("screenshots/" + std::to_string(++screenshot_counter) + ".png");
        }
	}

	bool onKeyDown(ui::Key key) override {
		if (key == ui::Key::Space) {
			paused = !paused;
		} else if (key == ui::Key::T && paused) {
			tick();
		} else if (key == ui::Key::G){
            gravity_dir = (gravity_dir + 1) % 4;
        } else if (key == ui::Key::R){
            recording = !recording;
        } else if (key == ui::Key::C){
			show_corners = !show_corners;
		} else if (key == ui::Key::D){
			show_contacts = !show_contacts;
		}
		return true;
	}

	void render(sf::RenderWindow& rw) override {

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
			shape->render(rw, show_corners);
            if (show_corners){
    			s.setPosition(shape->getBody().getPosition());
			    rw.draw(s);
            }
		}

		if (show_contacts){
			s.setOutlineColor(sf::Color(0xFF0000FF));

			// render collisions
			const auto& collisions = physics_engine.getCollisions();
			for (const auto& c : collisions){
				float rad = 2.0f * c.depth + 1.0f;
				s.setPosition(c.a.getPosition() + c.radius_a);
				s.setRadius(rad);
				s.setOrigin({rad, rad});

				rw.draw(s);
				sf::Vertex verts[2] = {
					{c.a.getPosition() + c.radius_a, sf::Color(0xFFFF)},
					{c.a.getPosition() + c.radius_a + c.normal * 30.0f, sf::Color(0xFF00)},
				};
				rw.draw(verts, 2, sf::PrimitiveType::Lines);
			}
		}

		if (!paused) {
			tick();
		}
	}

private:

	bool paused;
    bool recording;
    int screenshot_counter;
    int gravity_dir;
    bool show_corners;
	bool show_contacts;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};