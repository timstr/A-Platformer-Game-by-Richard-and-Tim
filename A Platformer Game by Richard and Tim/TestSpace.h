#pragma once
#include "space.h"

struct TestObstruction : Obstruction {
	TestObstruction(){
		image.loadFromFile("images/testmapboundary1.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.25;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct TestEntity : Entity {
	TestEntity(){
			
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sf::CircleShape circle;
		circle.setFillColor(sf::Color((uint32_t)std::hash<Entity*>{}(this) | 0xFF));
		for (Circle& c : circles){
			circle.setRadius(c.radius);
			circle.setPointCount(2 * pi * c.radius);
			circle.setPosition(position + offset - vec2(c.radius, c.radius) + c.center);
			rw.draw(circle);
		}
	}
};

struct SimpleEntity : TestEntity {
	SimpleEntity(){
		addCircle(Circle({0, 0}, 5 + rand() % 45));
		mass = 10.0;
	}
};

struct ComplexEntity : TestEntity {
	ComplexEntity(){
		addCircle(Circle({-25, -25}, 5 + rand() % 25));
		addCircle(Circle({25, -25}, 5 + rand() % 25));
		addCircle(Circle({-25, 25}, 5 + rand() % 25));
		addCircle(Circle({25, 25}, 5 + rand() % 25));
		mass = 20.0;
	}
};

struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 10;
		entities.resize(num_entities);
		for (int i = 0; i < num_entities; i++){
			entities[i] = new SimpleEntity();
			entities[i]->position = {500, 200};
			entities[i]->velocity = vec2((((rand() % 100) - 50) / 5.0), (((rand() % 100) - 50) / 5.0));
			addEntity(entities[i]);
		}

		ComplexEntity* entity = new ComplexEntity();
		entity->position = {500, 200};
		entity->velocity = vec2((((rand() % 100) - 50) / 5.0), (((rand() % 100) - 50) / 5.0));
		entities.push_back(entity);
		addEntity(entity);

		addObstruction(obstruction = new TestObstruction());
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		Space::render(rw, offset);
		vec2 normal = obstruction->getNormalAt(probe, {0.5, 0.5});
		sf::Vertex points[] = {
			sf::Vertex(probe + offset, sf::Color(0x00FF00FF)),
			sf::Vertex(probe + offset + 50.0f * normal, sf::Color(0x00FF00FF))
		};
		rw.draw(points, 2, sf::PrimitiveType::Lines);
	}

	std::vector<Entity*> entities;
	TestObstruction* obstruction;
	vec2 probe;
};