#pragma once

#include "BoundingBox.hpp"
#include "Vec2.hpp"

namespace phys {
		
	struct RigidBody {

		enum Type {
			Rectangle,
			Circle,
			Convex,
			Raster
		};

		RigidBody(Type _type, float _mass, float _moment, float _elasticity);
		virtual ~RigidBody();

		const Type type;

		// TODO: rename to restitution
		const float elasticity;

		const float mass;
		const float inverse_mass;

		const float moment;
		const float inverse_moment;
		
		// apply a force to the body at some general point in space
		void applyForceAt(vec2 force, vec2 point_world_space);

		// apply a force to the body's center of mass
		void applyForce(vec2 force);

		// apply a torque to the body
		void applyTorque(float torque);

		// apply an impulse to the body at some general point in space
		void applyImpulseAt(vec2 impulse, vec2 point_world_space);

		// apply an impulse to the body's center of mass
		void applyImpulse(vec2 impulse);

		// apply an angular impulse to the body
		void applyAngularImpulse(float impulse);

		// get the body's position
		const vec2& getPosition() const;

		// set the body's position
		void setPosition(vec2 _position);

		// get the body's velocity
		const vec2& getVelocity() const;

		// set the body's velocity
		void setVelocity(vec2 _velocity);

		// get the body's angle
		float getAngle() const;

		// set the body's angle
		void setAngle(float _angle);

		// get the body's angular velocity
		float getAngularVelocity() const;

		// set the body's angular velocity
		void setAngularVelocity(float _angular_velocity);

		// get the transformation mapping from world space to the body's local space
		const mat2x2& getTransform() const;

		// get the transformation mapping from the body's local space to world space
		const mat2x2& getInverseTransform() const;

		// get the smallest rectangle containing the body, in world space
		virtual BoundingBox getBoundingBox() const = 0;

		// get the forces currently acting on the body, plus the
		// effective forces from any impulses during the time frame
		vec2 getEffectiveForces(float dt) const;

		// get the torques currently acting on the body, plus the
		// effective torques from any angular impulses during the time frame
		float getEffectiveTorques(float dt) const;

	protected:

		// reset forces and impulses to zero
		void resetAccumulators();

		vec2 position;
		vec2 velocity;
		vec2 forces;
		vec2 impulses;

		float angle;
		float angular_velocity;
		float torques;
		float angular_impulses;

		mutable bool transform_needs_update;
		mutable mat2x2 transform;

		mutable bool inv_transform_needs_update;
		mutable mat2x2 inv_transform;

		friend struct Engine;
	};

} // namespace phys