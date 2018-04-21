#pragma once

#include "rigidbody.h"
#include "collision.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
#include <Eigen/Sparse>

namespace phys {

	struct Engine {

		Engine() :
			collision_table({
				{{RigidBody::Circle, RigidBody::Circle}, collisionCircleCircle},
				{{RigidBody::Circle, RigidBody::Rectangle}, collisionCircleRectangle},
				{{RigidBody::Rectangle, RigidBody::Circle}, collisionRectangleCircle},
				{{RigidBody::Rectangle, RigidBody::Rectangle}, collisionRectangleRectangle}
			}) {

		}

		void addRigidBody(RigidBody& body){
			bodies.emplace_back(&body);
		}
		void removeRigidBody(RigidBody& body){
			bodies.erase(std::remove(bodies.begin(),
									 bodies.end(),
									 &body),
						 bodies.end());
		}

		void tick(float dt){
			// user-defined forces should have been applied

			// accelerate and move all bodies
			for (const auto& body : bodies){
				body->velocity += body->forces * body->inverse_mass * dt;
				body->position += body->velocity * dt;
				body->angular_velocity += body->torques * body->inverse_moment * dt;
				body->angle += body->angular_velocity * dt;
				body->forces = {0.0f, 0.0f};
				body->torques = 0.0f;
			}

			// find collisions
			std::vector<Collision> collisions;

			for (int i = 0; i < bodies.size(); ++i){
				for (int j = i + 1; j < bodies.size(); ++j){
					if (bodies[i]->getBoundingBox().collidesWith(bodies[j]->getBoundingBox())){
						collisions.emplace_back(collide(*bodies[i], *bodies[j]));
					}
				}
			}

			// TODO: add custom constraints such as joints, springs, etc

			// TODO: solve constraints, apply corrective impulses/forces
			/*
			Needed:
			J - Jacobian matrix of constraint function w.r.t. state vector
			M - mass/moment matrix
			M^-1 - inverse mass/moment matrix
			dJ/dt - element-wise time derivate of Jacobian matrix J
			F_ext - vector of external forces (applied as desired between simulation ticks)

			*/

			using namespace Eigen;

			const int n = bodies.size();
			const int m = collisions.size();

			SparseMatrix<float> J {m, 3*n};
			SparseMatrix<float> Jdot {m, 3*n};

			SparseMatrix<float> M {3*n, 3*n};
			SparseMatrix<float> M_inv {3*n, 3*n};

		}

		private:

		Collision collide(RigidBody& a, RigidBody& b){
			auto pair = std::make_pair(a.type, b.type);
			auto it = collision_table.find(pair);
			if (it != collision_table.end()){
				return it->second(a, b);
			} else {
				throw std::runtime_error("The collision jump table was used incorrectly");
			}
		}

		bool possibleCollision(const RigidBody& a, const RigidBody& b){
			const BoundingBox abb = a.getBoundingBox();
			const BoundingBox bbb = b.getBoundingBox();
			return abb.collidesWith(bbb);
		}

		std::vector<RigidBody*> bodies;

		using CollisionFunction = Collision(*)(RigidBody&, RigidBody&);
		const std::map<std::pair<RigidBody::Type, RigidBody::Type>, CollisionFunction> collision_table;
	};

} // namespace phys