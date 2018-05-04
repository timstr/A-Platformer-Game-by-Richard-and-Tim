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

			// solve constraints, apply corrective forces

			using namespace Eigen;

			const int n = bodies.size();
			const int m = 2; // 2 test constraints for now

			SparseMatrix<float> J {m, 3*n}; // Jacobian of constraint function
			SparseMatrix<float> Jdot {m, 3*n}; // derivative of Jacobian w.r.t. time
			SparseMatrix<float> M_inv {3*n, 3*n}; // inverse mass/moment diagonal matrix

			VectorXf F_ext {3 * n}; // accumulated external forces
			VectorXf qdot {3 * n}; // velocities and angular velocities

			VectorXf C {m}; // Constraint functions
			VectorXf Cdot {m}; // derivative of constraint functions w.r.t. time

			// TODO: tune these
			float k_s = 0.005f; // feedback spring constant
			float k_d = 0.1f; // feedback damping constant


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
			{
				const int a = 0;
				const int b = 1;

				const vec2 dP = bodies[b]->position - bodies[a]->position;
				const vec2 dV = bodies[b]->velocity - bodies[a]->velocity;
				const float dist = 200.0f;

				// various derivates of the fixed-distance constraint function:
				//	C = (1/2)*(|Pb - Pa|^2 - d^2)

				const float dCdPx = -dP.x;
				const float dCdPy = -dP.y;

				const float d2CdPxdt = -dV.x;
				const float d2CdPydt = -dV.y;

				const float valC = 0.5f * ((dP.x * dP.x) + (dP.y * dP.y) - (dist * dist));
				const float valCdot = dP.x * dV.x + dP.y * dV.y;

				// initialize C and Cdot
				C(0) = valC;
				Cdot(0) = valCdot;

				// initialize J
				J.insert(0, 3 * a + 0) = dCdPx;
				J.insert(0, 3 * a + 1) = dCdPy;

				J.insert(0, 3 * b + 0) = -dCdPx;
				J.insert(0, 3 * b + 1) = -dCdPy;

				// initialize Jdot
				Jdot.insert(0, 3 * a + 0) = d2CdPxdt;
				Jdot.insert(0, 3 * a + 1) = d2CdPydt;

				Jdot.insert(0, 3 * b + 0) = -d2CdPxdt;
				Jdot.insert(0, 3 * b + 1) = -d2CdPydt;
			}

			// Test: single fixed distance constraint between body a and the point (500, 100)
			{
				const int a = 0;
				const int b = 1;

				const vec2 dP = vec2(500.0f, 100.0f) - bodies[a]->position;
				const vec2 dV = -bodies[a]->velocity;
				const float dist = 200.0f;

				// various derivates of the fixed-distance constraint function:
				//	C = (1/2)*(|Pb - Pa|^2 - d^2)

				const float dCdPx = -dP.x;
				const float dCdPy = -dP.y;

				const float d2CdPxdt = -dV.x;
				const float d2CdPydt = -dV.y;

				const float valC = 0.5f * ((dP.x * dP.x) + (dP.y * dP.y) - (dist * dist));
				const float valCdot = dP.x * dV.x + dP.y * dV.y;

				// initialize C and Cdot
				C(1) = valC;
				Cdot(1) = valCdot;

				// initialize J
				J.insert(1, 3 * a + 0) = dCdPx;
				J.insert(1, 3 * a + 1) = dCdPy;

				// initialize Jdot
				Jdot.insert(1, 3 * a + 0) = d2CdPxdt;
				Jdot.insert(1, 3 * a + 1) = d2CdPydt;
			}

			// calculate A and y to solve A*lambda = y
			SparseMatrix<float> A = J * M_inv * J.transpose();
			VectorXf y = -Jdot * qdot - J * M_inv * F_ext - k_s * C - k_d * Cdot;
			VectorXf lambda;

			SparseLU<SparseMatrix<float>> solver;
			solver.compute(A);
			if (solver.info() == Success){
				lambda = solver.solve(y);
				if (solver.info() == Success){

					// calculate corrective forces
					VectorXf F_c = J.transpose() * lambda;

					// apply corrective forces and torques
					for (int i = 0; i < n; ++i){
						// TEST:
						bodies[i]->ext_forces = bodies[i]->forces;
						bodies[i]->corr_forces = {F_c(3 * i + 0), F_c(3 * i + 1)};
						// end test
						bodies[i]->forces.x += F_c(3 * i + 0);
						bodies[i]->forces.y += F_c(3 * i + 1);
						bodies[i]->torques += F_c(3 * i + 2);
					}

				} else {
					//throw std::runtime_error("Constraint solving failed");
				}
			} else {
				//throw std::runtime_error("Constraint computation failed");
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