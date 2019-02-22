#include "PhysicsEngine.hpp"
#include "CollisionDetection.hpp"

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
		bodies.erase(std::remove(bodies.begin(), bodies.end(), &body), bodies.end());
	}

	void Engine::tick(float dt){
        collisions.clear(); // TODO: consider using previous collisions to aid detection
		detectCollisions();
		solveConstraints(dt);
        applyFriction();
		moveBodies(dt);
	}

	const std::vector<CollisionConstraint>& Engine::getCollisions() const {
		return collisions;
	}

	void Engine::detectCollisions() {
		// find collisions
		for (int i = 0; i < bodies.size(); ++i){
			for (int j = i + 1; j < bodies.size(); ++j){
				// TODO: double-check bounding box logic, render boxes around bodies to be sure
				//if (possibleCollision(*bodies[i], *bodies[j])){
					findCollisions(*bodies[i], *bodies[j]);
				//}
			}
		}
	}

	void Engine::solveConstraints(float dt) {
        const int num_iterations = 20; // TODO: make this configurable
        for (int i = 0; i < num_iterations; ++i){
			for (auto& c : collisions){
				c.solve(dt);
			}
			for (auto& c : constraints){
				c->solve(dt);
			}
        }
	}

	void Engine::applyFriction() {
		for (auto& c : collisions){
			float coeff = std::min(c.a.getFriction(), c.b.getFriction());
			float mag = c.impulse * coeff;
			vec2 tan = orthogonalCW(c.normal);
			vec2 vel_a = c.a.getVelocityAt(c.a.getPosition() + c.radius_a);
			vec2 vel_b = c.b.getVelocityAt(c.b.getPosition() + c.radius_b);
			vec2 rel_velo = vel_b - vel_a;
			float tan_velo = dot(rel_velo, tan);
			
			if (tan_velo < 0.0f){
				tan_velo *= -1.0f;
				tan *= -1.0f;
			}
			
			///////////////////////
			const float term_a = pow(cross(c.radius_a, tan), 2.0f) * c.a.inverse_moment;
			const float term_b = pow(cross(c.radius_b, tan), 2.0f) * c.b.inverse_moment;
			const float denom = c.a.inverse_mass + c.b.inverse_mass + term_a + term_b;
			///////////////////////

			// the friction impulse may not accelerate the bodies relative to eachother at the contact point, only decelerate
			// i.e. tan_velo may only decrease

			float tan_velo_delta = -mag * denom;
			float new_velo = std::max(tan_velo + tan_velo_delta, 0.0f);
			float delta = new_velo - tan_velo;
			float impulse = -delta / denom;
			c.a.applyImpulseAt(impulse * tan, c.a.getPosition() + c.radius_a);
			c.b.applyImpulseAt(-impulse * tan, c.b.getPosition() + c.radius_b);
		}
	}

	void Engine::moveBodies(float dt) {
		// accelerate and move all bodies
		for (const auto& body : bodies){
			body->setPosition(body->getPosition() + body->getVelocity() * dt);
			body->setAngle(body->getAngle() + body->getAngularVelocity() * dt);
		}
	}
	
	void Engine::findCollisions(RigidBody& a, RigidBody& b) {
		auto pair = std::make_pair(a.type, b.type);
		auto it = collision_table.find(pair);
		if (it != collision_table.end()){
			it->second(a, b, collisions);
		} else {
			throw std::runtime_error("The collision jump table was used incorrectly");
		}
	}

	bool Engine::possibleCollision(const RigidBody& a, const RigidBody& b) const {
		const BoundingBox bba = a.getBoundingBox();
		const BoundingBox bbb = b.getBoundingBox();
		return bba.collidesWith(bbb);
	}

} // namespace phys
