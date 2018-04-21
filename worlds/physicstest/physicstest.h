#pragma once

#include "physengine.h"
#include "gui/gui.h"

struct ShapeGuy {
	virtual ~ShapeGuy(){

	}

	virtual phys::RigidBody& getBody() = 0;

	virtual void draw(sf::RenderWindow& rw) = 0;

	sf::Color getColor() const {
		uint32_t hash = static_cast<uint32_t>(std::hash<ShapeGuy const*>{}(this));
		return sf::Color(hash | 0xFF);
	}
};

struct RectangleGuy : ShapeGuy {
	RectangleGuy(vec2 size, vec2 pos)
		: body(size, 1.0, 0.5) {
		body.setPosition(pos);
	}

	phys::RigidBody& getBody() override {
		return body;
	}

	void draw(sf::RenderWindow& rw) override {
		sf::RectangleShape rs;
		rs.setSize(body.size);
		rs.setOrigin(body.size * 0.5f);
		rs.setRotation(body.getAngle());
		rs.setPosition(body.getPosition());

		rs.setFillColor(getColor());
		rs.setOutlineColor(sf::Color(0xFF));
		rs.setOutlineThickness(1.0f);

		rw.draw(rs);
	}

	phys::Rectangle body;
};

struct CircleGuy : ShapeGuy {
	CircleGuy(float radius, vec2 pos)
		: body(radius, 1.0, 0.5) {
		body.setPosition(pos);
	}

	phys::RigidBody& getBody() override {
		return body;
	}

	void draw(sf::RenderWindow& rw) override {
		sf::CircleShape cs;
		cs.setRadius(body.radius);
		cs.setOrigin({body.radius, body.radius});
		cs.setRotation(body.getAngle());
		cs.setPosition(body.getPosition());

		cs.setFillColor(getColor());
		cs.setOutlineColor(sf::Color(0xFF));
		cs.setOutlineThickness(1.0f);

		rw.draw(cs);
	}

	phys::Circle body;
};

