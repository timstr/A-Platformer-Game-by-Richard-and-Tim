#pragma once
#include "space.h"

namespace Platformer {

	struct TestObstruction : Obstruction {
		TestObstruction(){
			image.loadFromFile("images/testmapboundary1.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);

			setImage(sprite);
			setBoundary(image);
		}

		private:
		sf::Image image;
		sf::Texture texture;
		sf::Sprite sprite;
	};

	struct TestEntity : Entity {
		const double radius = 20;

		TestEntity(){
			addCircle(Circle({0, 0}, radius));
			mass = 10;
		}

		void render(sf::RenderWindow& rw, vec2 offset) override {
			sf::CircleShape circle(radius, radius);
			circle.setPosition(position + offset - vec2(radius, radius));
			circle.setFillColor(sf::Color(0xFF0000FF));
			rw.draw(circle);
		}
	};

	struct TestSpace : Space {
		TestSpace(){
			addEntity(entity = new TestEntity());
			addObstruction(obstruction = new TestObstruction());
			entity->position = {500, 200};
			entity->velocity = vec2((((rand() % 100) - 50) / 5.0), (((rand() % 100) - 50) / 5.0));
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

		TestEntity* entity;
		TestObstruction* obstruction;
		vec2 probe;
	};

}