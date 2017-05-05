#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

namespace Platformer {

	typedef sf::Vector2f vec2;

	struct Obstruction;
	struct Map;
	struct Obstacle;

	struct Entity;
	struct Item;
	struct Character;
	struct Player;
	struct Creature;

	struct Space;

	/////////////////////////////////////////////////////////
	// In this header, all the basic mechanical components
	// shall be forward-declared, and their implementations
	// shall be found in the cpp files included at the end
	// of this file
	/////////////////////////////////////////////////////////

	// Renderable is the base class to all things having a visual presence
	struct Renderable {
		virtual ~Renderable(){

		}

		virtual void render(sf::RenderWindow rw, vec2 offset) = 0;
	};

	// Obstruction is the base class to all things that have
	// a complicated raster-image physical boundary with 
	// which entities are to interact physically
	struct Obstruction : Renderable {
		// test whether a point collides with the obstruction
		bool hitTest(vec2 point);

		// get the force exerted on a body colliding at the given point
		// moving at the given velocity having the given mass
		vec2 getCollisionForce(vec2 point, vec2 velocity, double mass);

		protected:

		// the spatial position
		vec2 pos;

		// the visual appearance
		sf::Sprite image;

		// the physical boundary
		sf::Image boundary;

		// the coefficient of friction
		// 0 for totally slippy
		// 1 for totally grippy
		double friction;
	};

	// Map is the static physical boundary of a space
	struct Map : Obstruction {
		
	};

	// Obstacle is a component of a space with unique physical
	// properties different from the map they may be found with
	struct Obstacle : Obstruction {
		bool destructible;
		int healthpoints;
	};


	// Entity is the base class to all things that interact with such
	// obstructions as the map and obstacles
	// an Entity has a visual appearance, a rigid physical boundary
	// comprised of one or more circles, and physical mass
	struct Entity : Renderable {

		// possibly collide with obstruction and change
		// velocity accordingly
		void tryCollisionWith(Obstruction* obstruction);

		protected:

		struct Circle {
			Circle(vec2 _center = {0, 0}, double _radius = 20.0){
				center = _center;
				radius = _radius;
			}
			vec2 center;
			double radius;
		};

		std::vector<Circle> circles;

		double mass;
	};

	// an Item is a floating, kinetic representation of an inventory item
	// that can be collected by a player
	struct Item : Entity {

	};

	// Character is the base class to all interactive beings
	struct Character : Entity {

	};

	// Player is the character that is controlled by the user
	struct Player : Character {

	};

	// Creature is an autonomous character that moves around
	// independently and can interact with other characters
	struct Creature : Character {

	};



	// a Space is all the above-mentioned components brought together
	// where interactions and spatial relations are collectively managed
	// A Space consists of a map, a number of obstacles, and a number of
	// entities
	struct Space {

		

	};

}

#include "obstruction.cpp"
#include "map.cpp"
#include "obstacle.cpp"
#include "entity.cpp"
#include "item.cpp"
#include "character.cpp"
#include "player.cpp"
#include "creature.cpp"
