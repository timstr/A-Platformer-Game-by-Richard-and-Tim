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
		timestamp(0.0f),
		recording(false),
		screenshot_counter(0),
		show_corners(false),
		show_contacts(false) {

		setSize(_size, true);

		// friction test
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{200.0f, 100.0f}, vec2{150.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			//auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{300.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{150.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s2->body.setAngularVelocity(0.1f);
			s1->body.setAngle(pi * -0.125f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{400.0f, 100.0f}, vec2{500.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<RectangleGuy>(vec2{100.0f, 100.0f}, vec2{500.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			//auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{500.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s1->body.setAngle(pi * 0.125f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}
		{
			auto s1 = std::make_unique<RectangleGuy>(vec2{200.0f, 100.0f}, vec2{850.0f, 400.0f}, sf::Color(0x808080FF), 0.0f);
			auto s2 = std::make_unique<CircleGuy>(25.0f, vec2{850.0f, 200.0f}, sf::Color(0xC0C0C0FF));
			s1->body.setAngle(pi * 0.125f);
			addBody(std::move(s1));
			addBody(std::move(s2));
		}

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

		// many random shapes
        /*{
            int n = 10;
            const float rad = 10.0f;
			const int cols = 25;
			const float padding = 30.0f;

            std::uniform_real_distribution<float> dist {0.0f, 1.0f};
			std::uniform_int_distribution<int> bdist {0, 1};
			std::uniform_int_distribution<int> idist {0, 255};

            for (int i = 0; i < n; ++i){
                std::unique_ptr<ShapeGuy> shape;
                vec2 pos;// = vec2(_size.x * dist(rand_eng), _size.y * dist(rand_eng));
                {
                    const float spacing = (_size.x - 2.0f * padding) / (float)cols;
                    pos.x = ((float)(i % cols) + 0.5f) * spacing + padding + dist(rand_eng) * 0.001f;
                    pos.y = ((float)(i / cols) + 0.5f) * spacing + padding + dist(rand_eng) * 0.001f;
                }

				auto color = sf::Color(idist(rand_eng), idist(rand_eng), idist(rand_eng), 255);

                if (bdist(rand_eng)){
                    shape = std::make_unique<CircleGuy>(rad + dist(rand_eng) * 10.0f, pos, color);
                } else {
                    shape = std::make_unique<RectangleGuy>(vec2(2.0f * rad + dist(rand_eng) * 10.0f, 2.0f * rad + dist(rand_eng) * 10.0f), pos, color);
                }

                //vec2 velo = shape->getBody().getPosition() - _size * 0.5f;
                //velo *= -0.5f / abs(velo);
                //shape->getBody().setVelocity(velo);

                //shape->getBody().setAngle(dist(rand_eng) * 2.0f * pi);
                //shape->getBody().setAngularVelocity((-1.0f + 2.0f * dist(rand_eng)) * 0.1f);

                physics_engine.addRigidBody(shape->getBody());

                shapes.emplace_back(std::move(shape));
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
		/*auto topwall = std::make_unique<RectangleGuy>(vec2{_size.x, 10.0f}, vec2{_size.x * 0.5f, 5.0f}, sf::Color(0x804040FF), 0.0f);
		auto bottomwall = std::make_unique<RectangleGuy>(vec2{_size.x, 10.0f}, vec2{_size.x * 0.5f, _size.y - 5.0f}, sf::Color(0x804040FF), 0.0f);
		auto leftwall = std::make_unique<RectangleGuy>(vec2{10.0f, _size.y - 20.0f}, vec2{5.0f, _size.y * 0.5f}, sf::Color(0x804040FF), 0.0f);
		auto rightwall = std::make_unique<RectangleGuy>(vec2{10.0f, _size.y - 20.0f}, vec2{_size.x - 5.0f, _size.y * 0.5f}, sf::Color(0x804040FF), 0.0f);
		physics_engine.addRigidBody(topwall->getBody());
		physics_engine.addRigidBody(bottomwall->getBody());
		physics_engine.addRigidBody(leftwall->getBody());
		physics_engine.addRigidBody(rightwall->getBody());
		shapes.emplace_back(std::move(topwall));
		shapes.emplace_back(std::move(bottomwall));
		shapes.emplace_back(std::move(leftwall));
		shapes.emplace_back(std::move(rightwall));*/

        // label
        label = add<ui::FreeElement>();
        label->setPos({20.0f, 20.0f});
	}
	~PhysicsTestUI() {
		for (const auto& shape : shapes) {
			physics_engine.removeRigidBody(shape->getBody());
		}
		shapes.clear();
	}

	void addBody(std::unique_ptr<ShapeGuy> shape){
		physics_engine.addRigidBody(shape->getBody());
		shapes.emplace_back(std::move(shape));
	}

	void tick(){

        // slow motion if shift is held
        float delta = (keyDown(ui::Key::LShift) || keyDown(ui::Key::RShift)) ? 0.1f : 1.0f;

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

        const float now = ui::getProgramTime();
        const int fps = (int)std::floor(1.0f / (now - timestamp));
        timestamp = now;

        label->clear();
        label->write(std::to_string(fps), getFont());

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

	ui::Ref<ui::Element> label;
	bool paused;
    bool recording;
    int screenshot_counter;
    int gravity_dir;
    float timestamp;
    bool show_corners;
	bool show_contacts;
	std::vector<std::unique_ptr<ShapeGuy>> shapes;
	phys::Engine physics_engine;
};