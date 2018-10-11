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

		const float elasticity;

		const float mass;
		const float inverse_mass;

		const float moment;
		const float inverse_moment;
		
		void applyForceAt(vec2 force, vec2 point_world_space);

		void applyForce(vec2 force);

		void applyTorque(float torque);

		const vec2& getPosition() const;
		void setPosition(vec2 _position);

		const vec2& getVelocity() const;
		void setVelocity(vec2 _velocity);

		float getAngle() const;
		void setAngle(float _angle);

		float getAngularVelocity() const;
		void setAngularVelocity(float _angular_velocity);

		const mat2x2& getTransform() const;

		const mat2x2& getInverseTransform() const;

		virtual BoundingBox getBoundingBox() const = 0;

		protected:

		vec2 position;
		vec2 velocity;
		vec2 forces;

		float angle;
		float angular_velocity;
		float torques;

		mutable bool transform_needs_update;
		mutable mat2x2 transform;

		mutable bool inv_transform_needs_update;
		mutable mat2x2 inv_transform;

		friend struct Engine;
	};

} // namespace phys