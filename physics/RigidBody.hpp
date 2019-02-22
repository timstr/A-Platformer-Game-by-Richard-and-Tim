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

		// TODO: make the following const members into non-const private members
		// with get/set functions

		// TODO: rename to restitution
		const float elasticity;

		const float mass;
		const float inverse_mass;

		const float moment;
		const float inverse_moment;
		
		// set the body's coefficient of friction
		void setFriction(float friction);

		// get the body's coefficient of friction
		float getFriction() const;

		// apply a force to the body at some general point in space
		void applyForceAt(vec2 force, vec2 point_world_space, float dt);

		// apply a force to the body's center of mass
		void applyForce(vec2 force, float dt);

		// apply a torque to the body
		void applyTorque(float torque, float dt);

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

		// get the body's velocity at the center of mass
		const vec2& getVelocity() const;

		// get the body's velocity at a specific point, in world coordinates
		const vec2 getVelocityAt(vec2 point_world_space) const;

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
        
	private:

		vec2 position;
		vec2 velocity;

		float angle;
		float angular_velocity;

		mutable bool transform_needs_update;
		mutable mat2x2 transform;

		mutable bool inv_transform_needs_update;
		mutable mat2x2 inv_transform;

		float friction;
	};

} // namespace phys