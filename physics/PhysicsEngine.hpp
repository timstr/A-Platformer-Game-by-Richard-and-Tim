#pragma once

#include "RigidBody.hpp"
#include "Collision.hpp"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>

namespace phys {

	struct Engine {

		Engine();

		void addRigidBody(RigidBody& body);
		void removeRigidBody(RigidBody& body);

		void tick(float dt);

		private:

		Collision collide(RigidBody& a, RigidBody& b);

		bool possibleCollision(const RigidBody& a, const RigidBody& b) const;

		std::vector<RigidBody*> bodies;

		using CollisionFunction = Collision(*)(RigidBody&, RigidBody&);
		const std::map<std::pair<RigidBody::Type, RigidBody::Type>, CollisionFunction> collision_table;
	};

} // namespace phys