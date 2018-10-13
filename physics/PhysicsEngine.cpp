#include "PhysicsEngine.hpp"

namespace phys {

	Engine::Engine() :
		collision_table({
			{{RigidBody::Circle, RigidBody::Circle}, collideCircleCircle},
			{{RigidBody::Circle, RigidBody::Rectangle}, collideCircleRectangle},
			{{RigidBody::Circle, RigidBody::Convex}, collideCircleConvex},
			{{RigidBody::Circle, RigidBody::Raster}, collideCircleRaster},

			{{RigidBody::Rectangle, RigidBody::Circle}, collideRectangleCircle},
			{{RigidBody::Rectangle, RigidBody::Rectangle}, collideRectangleRectangle},
			{{RigidBody::Rectangle, RigidBody::Convex}, collideRectangleConvex},
			{{RigidBody::Rectangle, RigidBody::Raster}, collideRectangleRaster},

			{{RigidBody::Convex, RigidBody::Circle}, collideConvexCircle},
			{{RigidBody::Convex, RigidBody::Rectangle}, collideConvexRectangle},
			{{RigidBody::Convex, RigidBody::Convex}, collideConvexConvex},
			{{RigidBody::Convex, RigidBody::Raster}, collideConvexRaster},

			{{RigidBody::Raster, RigidBody::Circle}, collideRasterCircle},
			{{RigidBody::Raster, RigidBody::Rectangle}, collideRasterRectangle},
			{{RigidBody::Raster, RigidBody::Convex}, collideRasterConvex},
			{{RigidBody::Raster, RigidBody::Raster}, collideRasterConvex},
		}) {

	}

	void Engine::addRigidBody(RigidBody& body){
		bodies.emplace_back(&body);
	}
	void Engine::removeRigidBody(RigidBody& body){
		bodies.erase(std::remove(bodies.begin(),
			bodies.end(),
			&body),
			bodies.end());
	}

	void Engine::tick(float dt){

		// user-defined forces should have been applied

		// find collisions
		std::vector<Collision> collisions;

		for (int i = 0; i < bodies.size(); ++i){
			for (int j = i + 1; j < bodies.size(); ++j){
				if (bodies[i]->getBoundingBox().collidesWith(bodies[j]->getBoundingBox())){
					collisions.emplace_back(collide(*bodies[i], *bodies[j]));
				}
			}
		}

		// resolve collisions by applying impulses
		for (const auto& collision : collisions){
			
		}

		// TODO: add custom constraints such as joints, springs, etc

		// solve constraints, apply corrective forces

		using namespace Eigen;

		const int n = (int)bodies.size();
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
			const vec2 forces = bodies[i]->getEffectiveForces(dt);
			const float torques = bodies[i]->getEffectiveTorques(dt);
			F_ext(3 * i + 0) = forces.x;
			F_ext(3 * i + 1) = forces.y;
			F_ext(3 * i + 2) = torques;
		}

		// initialize qdot
		for (int i = 0; i < n; ++i){
			qdot(3 * i + 0) = bodies[i]->velocity.x;
			qdot(3 * i + 1) = bodies[i]->velocity.y;
			qdot(3 * i + 2) = bodies[i]->angular_velocity;
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
					const vec2 forces = {F_c(3 * i + 0), F_c(3 * i + 1)};
					const float torques = F_c(3 * i + 2);
					bodies[i]->applyForce(forces);
					bodies[i]->applyTorque(torques);
				}

			} else {
				//throw std::runtime_error("Constraint solving failed");
			}
		} else {
			//throw std::runtime_error("Constraint computation failed");
		}

		// accelerate and move all bodies
		for (const auto& body : bodies){
			body->velocity += body->forces * body->inverse_mass * dt + body->impulses;
			body->position += body->velocity * dt;
			body->angular_velocity += body->torques * body->inverse_moment * dt + body->angular_impulses;
			body->angle += body->angular_velocity * dt;
			body->resetAccumulators();
		}
	}
	
	Collision Engine::collide(RigidBody& a, RigidBody& b){
		auto pair = std::make_pair(a.type, b.type);
		auto it = collision_table.find(pair);
		if (it != collision_table.end()){
			return it->second(a, b);
		} else {
			throw std::runtime_error("The collision jump table was used incorrectly");
		}
	}

	bool Engine::possibleCollision(const RigidBody& a, const RigidBody& b) const {
		const BoundingBox abb = a.getBoundingBox();
		const BoundingBox bbb = b.getBoundingBox();
		return abb.collidesWith(bbb);
	}

} // namespace phys