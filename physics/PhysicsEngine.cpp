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

	void Engine::addRigidBody(RigidBody* body){
		bodies.emplace_back(body);
	}
	void Engine::removeRigidBody(RigidBody* body){
		bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
	}

	void Engine::addConstraint(std::unique_ptr<Constraint> constraint) {
		constraints.push_back(std::move(constraint));
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
				if (possibleCollision(*bodies[i], *bodies[j])){
					findCollisions(*bodies[i], *bodies[j]);
				}
			}
		}
	}

	void Engine::solveConstraints(float dt) {
        const int num_iterations = 1;//50; // TODO: make this configurable
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
			float tan_velo = dot(c.b.getVelocityAt(c.radius_b) - c.a.getVelocityAt(c.radius_a), tan);
			
			if (tan_velo < 0.0f){
				tan_velo *= -1.0f;
				tan *= -1.0f;
			}

			const float inv_mass = c.a.getApparentInverseMassAt(c.radius_a, tan) + c.b.getApparentInverseMassAt(c.radius_b, tan);
			if (abs(inv_mass) < 1e-6f){
				continue;
			}

			float tan_velo_delta = -mag * inv_mass;
			float new_velo = std::max(tan_velo + tan_velo_delta, 0.0f);
			float delta = new_velo - tan_velo;
			float impulse = -delta / inv_mass;
			c.a.applyImpulseAt(impulse * tan, c.radius_a);
			c.b.applyImpulseAt(-impulse * tan, c.radius_b);
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
