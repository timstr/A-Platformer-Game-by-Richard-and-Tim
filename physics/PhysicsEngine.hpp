#pragma once

#include "RigidBody.hpp"
#include "Collision.hpp"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>

namespace phys {

	struct Engine {

		Engine();

		void addRigidBody(RigidBody& body);
		void removeRigidBody(RigidBody& body);

		void tick(float dt);

		// TODO: remove after testing
		const std::vector<Collision>& getCollisions() const;

	private:

		void resolveCollisions();

		void solveConstraints(float dt);

		void moveBodies(float dt);

		MaybeCollision findCollision(RigidBody& a, RigidBody& b);

		bool possibleCollision(const RigidBody& a, const RigidBody& b) const;

		void applyImpulse(const Collision& collision);

		std::vector<RigidBody*> bodies;

		using CollisionFunction = MaybeCollision(*)(RigidBody&, RigidBody&);
		const std::map<std::pair<RigidBody::Type, RigidBody::Type>, CollisionFunction> collision_table;

		std::vector<Collision> collisions;
	};

} // namespace phys