#pragma once

#include "rigidbody.h"
#include "collision.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>

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
			
			// find collisions
			/*std::vector<Collision> collisions;

			for (int i = 0; i < bodies.size(); ++i){
				for (int j = i + 1; j < bodies.size(); ++j){
					if (bodies[i]->getBoundingBox().collidesWith(bodies[j]->getBoundingBox())){
						collisions.emplace_back(collide(*bodies[i], *bodies[j]));
					}
				}
			}*/

			// TODO: add custom constraints such as joints, springs, etc

			// TODO: solve constraints, apply corrective impulses/forces
			/*
			Needed:
			J - Jacobian matrix of constraint function w.r.t. state vector
			Jdot - element-wise time derivate of Jacobian matrix J
			M - mass/moment matrix
			M^-1 - inverse mass/moment matrix
			dJ/dt - element-wise time derivate of Jacobian matrix J
			F_ext - vector of external forces (applied as desired between simulation ticks)
			qdot - velocity vector, time derivative of q (state vector)

			*/

			using namespace Eigen;

			const int n = bodies.size();
			const int m = 1; // 1 test constraint for now

			SparseMatrix<float> J {m, 3*n};
			SparseMatrix<float> Jdot {m, 3*n};

			VectorXf F_ext {3 * n};
			VectorXf qdot {3 * n};

			SparseMatrix<float> M_inv {3*n, 3*n};

			// initialize M_inv
			for (int i = 0; i < n; ++i){
				M_inv.insert(3 * i + 0, 3 * i + 0) = bodies[i]->inverse_mass;
				M_inv.insert(3 * i + 1, 3 * i + 1) = bodies[i]->inverse_mass;
				M_inv.insert(3 * i + 2, 3 * i + 2) = bodies[i]->inverse_moment;
			}

			// initialize F_ext
			for (int i = 0; i < n; ++i){
				F_ext(3 * i + 0) = bodies[i]->forces.x;
				F_ext(3 * i + 1) = bodies[i]->forces.y;
				F_ext(3 * i + 2) = bodies[i]->torques;
			}

			// initialize qdot
			for (int i = 0; i < n; ++i){
				qdot(3 * i + 0) = bodies[i]->velocity.x;
				qdot(3 * i + 1) = bodies[i]->velocity.y;
				qdot(3 * i + 2) = bodies[i]->angular_velocity;
			}

			// Test: single fixed distance constraint between bodies a and b
			const int a = 0;
			const int b = 1;

			// initialize J
			J.insert(0, 3 * a + 0) = bodies[a]->position.x - bodies[b]->position.x;		// dC/dPax
			J.insert(0, 3 * a + 1) = bodies[a]->position.y - bodies[b]->position.y;		// dC/dPay

			J.insert(0, 3 * b + 0) = bodies[b]->position.x - bodies[a]->position.x;		// dC/dPbx
			J.insert(0, 3 * b + 1) = bodies[b]->position.y - bodies[a]->position.y;		// dC/dPby
			
			// initialize Jdot
			Jdot.insert(0, 3 * a + 0) = bodies[a]->velocity.x - bodies[b]->velocity.x;	// d2C/dPaxdt
			Jdot.insert(0, 3 * a + 1) = bodies[a]->velocity.y - bodies[b]->velocity.y;	// d2C/dPaydt
			
			Jdot.insert(0, 3 * b + 0) = bodies[b]->velocity.x - bodies[a]->velocity.x;	// d2C/dPbxdt
			Jdot.insert(0, 3 * b + 1) = bodies[b]->velocity.y - bodies[a]->velocity.y;	// d2C/dPbydt

			// calculate A and y to solve A*lambda = y
			SparseMatrix<float> A = J * M_inv * J.transpose();
			VectorXf y = -Jdot * qdot - J * M_inv * F_ext;
			VectorXf lambda;

			SparseLU<SparseMatrix<float>> solver;
			solver.compute(A);
			if (solver.info() != Success){
				throw std::runtime_error("Constraint computation failed");
			}
			
			lambda = solver.solve(y);
			if (solver.info() != Success){
				throw std::runtime_error("Constraint solving failed");
			}

			// calculate corrective forces
			VectorXf f_corr = J.transpose() * lambda;

			// apply corrective forces and torques
			for (int i = 0; i < n; ++i){
				bodies[i]->forces.x += f_corr(3 * i + 0);
				bodies[i]->forces.y += f_corr(3 * i + 1);
				bodies[i]->torques += f_corr(3 * i + 2);
			}


			// accelerate and move all bodies
			for (const auto& body : bodies){
				body->velocity += body->forces * body->inverse_mass * dt;
				body->position += body->velocity * dt;
				body->angular_velocity += body->torques * body->inverse_moment * dt;
				body->angle += body->angular_velocity * dt;
				body->forces = {0.0f, 0.0f};
				body->torques = 0.0f;
			}
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