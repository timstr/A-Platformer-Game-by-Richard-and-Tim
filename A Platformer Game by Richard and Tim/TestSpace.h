#pragma once
#include "space.h"
#include "map.h"
#include "obstacle.h"

struct TestMap : Map {
	TestMap(){
		image.loadFromFile("images/testmapboundary3.png");
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

struct TreeObstacle : Obstacle {
	TreeObstacle(){
		image.loadFromFile("images/testobstacle1.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.0;
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct BoostObstacle : Obstacle {
	BoostObstacle(){
		image.loadFromFile("images/testobstacle2.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 0.0;
	}

	vec2 getContactAcceleration(const Entity* entity, vec2 normal) const override {
		return {2, 0};
	}

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct MovingObstacle : Obstacle {
	MovingObstacle(vec2 _position1, vec2 _position2, double frames){
		speed = 2 * pi / frames;

		position1 = _position1;
		position2 = _position2;

		image.loadFromFile("images/testobstacle3.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		setImage(sprite);
		setBoundary(image);
		friction = 1.0;
	}

	void tick() override {
		phase += speed;
		position = getPosition(phase);
	}

	private:

	vec2 getPosition(double _phase) const {
		return position1 + (position2 - position1) * (float)(0.5 + 0.5 * sin(_phase));
	}

	double phase;
	double speed;
	vec2 position1, position2;

	private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

struct TestEntity : Entity {
	TestEntity(){
		position = {700, 400};
		elasticity = (rand() % 100) * 0.01;
		friction = (rand() % 100) * 0.01;
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		sf::CircleShape circle;
		circle.setFillColor(sf::Color(255 * friction, 255 * elasticity, 127, 255));
		circle.setOutlineThickness(0.0);
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

struct GuyEntity : TestEntity {
	GuyEntity(){
		addCircle(Circle({0, -20}, 20));
		addCircle(Circle({0, 10}, 15));
		addCircle(Circle({-20, 5}, 10));
		addCircle(Circle({20, 5}, 10));
		addCircle(Circle({-10, 40}, 10));
		addCircle(Circle({10, 40}, 10));
		friction = 0.3;
		elasticity = 0.1;
	}

	vec2 getContactAcceleration(const Obstruction* obstruction, vec2 normal) const override {
		vec2 accel = {0, 0};
		accel += run_speed * vec2(1, 0);
		accel += vec2(0, jump_speed * -std::max(0.0, dot(normal, {0, -1})));
		return accel;
	}

	void updateMoves(float _run_speed, float _jump_speed){
		run_speed = _run_speed;
		jump_speed = _jump_speed;
	}

	float run_speed = 0.0f;
	float jump_speed = 0.0f;
};

struct TestSpace : Space {
	TestSpace(){
		const int num_entities = 10;
		const int test_entites = 0;

		for (int i = 0; i < num_entities; i++){
			Entity* entity = new SimpleEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			addEntity(entity);
			entities.push_back(entity);
		}

		for (int i = 0; i < test_entites; i++){
			Entity* entity = new ComplexEntity();
			entity->velocity = vec2((((rand() % 100) - 50) / 10.0), (((rand() % 100) - 50) / 10.0));
			addEntity(entity);
			entities.push_back(entity);
		}

		guy = new GuyEntity();
		addEntity(guy);
		entities.push_back(guy);

		addObstruction(map = new TestMap());

		addObstruction(tree = new TreeObstacle());
		tree->position = {250, 500};

		addObstruction(boost = new BoostObstacle());
		boost->position = {600, 500};

		addObstruction(mover = new MovingObstacle({400, 350}, {400, 475}, 100));
	}

	void render(sf::RenderWindow& rw, vec2 offset) override {
		Space::render(rw, offset);
	}

	void createEntity(vec2 position){
		SimpleEntity* entity = new SimpleEntity();
		entity->position = position;
		addEntity(entity);
		entities.push_back(entity);
	}

	std::vector<Entity*> entities;
	GuyEntity* guy;
	TestMap* map;
	TreeObstacle* tree;
	BoostObstacle* boost;
	MovingObstacle* mover;
};