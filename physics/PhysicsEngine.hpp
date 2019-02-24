#pragma once

#include "RigidBody.hpp"
#include "RectangleBody.hpp"
#include "CircleBody.hpp"
#include "ConvexBody.hpp"
#include "RasterBody.hpp"
#include "Constraint.hpp"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>

namespace phys {

	struct Engine {

		Engine();

		void addRigidBody(RigidBody* body);
		void removeRigidBody(RigidBody* body);

		void addConstraint(std::unique_ptr<Constraint> constraint);

		void tick(float dt);

		// TODO: remove after testing
		const std::vector<CollisionConstraint>& getCollisions() const;

	private:

        // search for collisions and add constraints as needed
        void detectCollisions();

        // globally solve all constraints iteratively
		void solveConstraints(float dt);

		// apply friction to contacts using the impulses calculated by constraint solving
		void applyFriction();

        // displace and rotate bodies according to their velocities
		void moveBodies(float dt);

        void findCollisions(RigidBody& a, RigidBody& b);

		bool possibleCollision(const RigidBody& a, const RigidBody& b) const;

		std::vector<RigidBody*> bodies;

		using CollisionFunction = void(*)(RigidBody&, RigidBody&, std::vector<CollisionConstraint>&);
		const std::map<std::pair<RigidBody::Type, RigidBody::Type>, CollisionFunction> collision_table;

		std::vector<CollisionConstraint> collisions;
		std::vector<std::unique_ptr<Constraint>> constraints;
	};

} // namespace phys