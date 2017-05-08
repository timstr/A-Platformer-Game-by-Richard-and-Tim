#pragma once
#include "space.h"

struct TestObstruction : Obstruction {
	TestObstruction(){
		image.loadFromFile("images/testmapboundary1.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 1.0;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct TestEntity : Entity {
	TestEntity(){
		position = {500, 200};
		elasticity = (rand() % 100) * 0.01;
		friction = (rand() % 100) * 0.01;
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sf::CircleShape circle;
		//circle.setFillColor(sf::Color((uint32_t)std::hash<Entity*>{}(this) | 0xFF));
		circle.setFillColor(sf::Color(255 * friction, 255 * elasticity, 0, 255));
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
		addCircle(Circle({0, 0}, 10 + rand() % 20));
		mass = 10.0;
	}
};

struct ComplexEntity : TestEntity {
	ComplexEntity(){
		int count = (rand() % 5) + 1;
		for (int i = 0; i < count; i++){
			addCircle(Circle(vec2(-25 + rand() % 51, -25 + rand() % 51), 5 + rand() % 25));
		}
		mass = 20.0;
	}
};

struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 50;
		const int test_entites = 0;

		entities.reserve(num_entities + test_entites);

		for (int i = 0; i < num_entities; i++){
			Entity* entity = new SimpleEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 5.0), (((rand() % 100) - 50) / 5.0));
			addEntity(entity);
			entities.push_back(entity);
		}

		for (int i = 0; i < test_entites; i++){
			Entity* entity = new ComplexEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 5.0), (((rand() % 100) - 50) / 5.0));
			addEntity(entity);
			entities.push_back(entity);
		}

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