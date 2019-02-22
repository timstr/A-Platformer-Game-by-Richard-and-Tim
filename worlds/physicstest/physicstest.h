#pragma once

#include "PhysicsEngine.hpp"
#include "gui/gui.h"

struct ShapeGuy {
	ShapeGuy(sf::Color color) : m_color(color) {
	
	}
	virtual ~ShapeGuy(){

	}

	virtual phys::RigidBody& getBody() = 0;

	virtual void render(sf::RenderWindow& rw, bool show_corners) = 0;

	sf::Color getColor() const {
		//uint32_t hash = static_cast<uint32_t>(std::hash<ShapeGuy const*>{}(this));
		//return sf::Color(hash | 0xFF);
		return m_color;
	}

private:
	sf::Color m_color;
};

struct RectangleGuy : ShapeGuy {
	RectangleGuy(vec2 size, vec2 pos, sf::Color color, float density = 1.0f)
		: ShapeGuy(color), body(size, density, 0.5f) {
		body.setPosition(pos);
	}

	phys::RigidBody& getBody() override {
		return body;
	}

	void render(sf::RenderWindow& rw, bool show_corners) override {
		sf::RectangleShape rs;
		rs.setSize(body.size());
		rs.setOrigin(body.size() * 0.5f);
		rs.setRotation(-body.getAngle() * 180.0f / pi);
		rs.setPosition(body.getPosition());

		rs.setFillColor(getColor());
		rs.setOutlineColor(sf::Color(0xFF));
		rs.setOutlineThickness(1.0f);

		rw.draw(rs);

        if (show_corners){
		    sf::CircleShape s;
		    s.setFillColor(sf::Color(0xFF));
		    s.setRadius(2.0f);
		    s.setOrigin({2.0f, 2.0f});
		    s.setPosition(body.getPosition() + body.getInverseTransform() * (0.5f * vec2{-body.size().x, -body.size().y}));
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * (0.5f * vec2{-body.size().x, body.size().y}));
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * (0.5f * vec2{body.size().x, -body.size().y}));
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * (0.5f * vec2{body.size().x, body.size().y}));
		    rw.draw(s);
        }
	}

	phys::RectangleBody body;
};

struct CircleGuy : ShapeGuy {
	CircleGuy(float radius, vec2 pos, sf::Color color, float density = 1.0f)
		: ShapeGuy(color), body(radius, density, 0.5f) {
		body.setPosition(pos);
	}

	phys::RigidBody& getBody() override {
		return body;
	}

	void render(sf::RenderWindow& rw, bool show_corners) override {
		sf::CircleShape cs;
		cs.setRadius(body.radius());
		cs.setOrigin({body.radius(), body.radius()});
		cs.setRotation(body.getAngle() * 180.0f / pi);
		cs.setPosition(body.getPosition());

		cs.setFillColor(getColor());
		cs.setOutlineColor(sf::Color(0xFF));
		cs.setOutlineThickness(1.0f);

		rw.draw(cs);

        if (show_corners){
		    sf::CircleShape s;
		    s.setFillColor(sf::Color(0xFF));
		    s.setRadius(2.0f);
		    s.setOrigin({2.0f, 2.0f});
		    s.setPosition(body.getPosition() + body.getInverseTransform() * vec2{-body.radius(), 0.0f});
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * vec2{body.radius(), 0.0f});
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * vec2{0.0f, -body.radius()});
		    rw.draw(s);
		    s.setPosition(body.getPosition() + body.getInverseTransform() * vec2{0.0f, body.radius()});
		    rw.draw(s);
        }
	}

	phys::CircleBody body;
};

