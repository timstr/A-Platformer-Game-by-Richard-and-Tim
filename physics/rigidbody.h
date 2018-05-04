#pragma once

#include "vec2.h"

namespace phys {



	struct BoundingBox {
		BoundingBox(vec2 _min, vec2 _max) : min(_min), max(_max) {

		}
		BoundingBox(float minx, float miny, float maxx, float maxy) : min(minx, miny), max(maxx, maxy) {

		}

		bool collidesWith(const BoundingBox& other) const {
			return
				max.x > other.min.x &&
				max.y > other.min.y &&
				min.x < other.max.x &&
				min.y < other.max.y;
		}

		vec2 min;
		vec2 max;
	};

	// TODO: add convex polygon, raster shape
	
	struct RigidBody {

		enum Type {
			Rectangle,
			Circle
		};

		RigidBody(Type _type, float _mass, float _moment, float _elasticity) :
			type(_type),
			velocity({0.0f, 0.0f}),
			position({0.0f, 0.0f}),
			forces({0.0f, 0.0f}),
			angle(0.0f),
			angular_velocity(0.0f),
			torques(0.0f),
			mass(_mass),
			inverse_mass(_mass == 0.0f ? 0.0f : (1.0f / _mass)),
			moment(_moment),
			inverse_moment(_moment == 0.0f ? 0.0f : (1.0f / _moment)),
			elasticity(_elasticity),
			transform_needs_update(true),
			inv_transform_needs_update(true),
			ext_forces({0, 0}), corr_forces({0, 0}) {

		}
		virtual ~RigidBody(){

		}

		// TEST:
		vec2 ext_forces;
		vec2 corr_forces;

		const Type type;

		const float elasticity;

		const float mass;
		const float inverse_mass;

		const float moment;
		const float inverse_moment;
		
		void applyForceAt(vec2 force, vec2 point_world_space){
			forces += force;
			vec2 rad = (point_world_space - position);
			torques += rad.x * force.y - rad.y * force.x;
		}

		void applyForce(vec2 force){
			forces += force;
		}

		void applyTorque(float torque){
			torques += torque;
		}

		const vec2& getPosition() const {
			return position;
		}
		void setPosition(vec2 _position){
			position = _position;
		}

		const vec2& getVelocity() const {
			return velocity;
		}
		void setVelocity(vec2 _velocity){
			velocity = _velocity;
		}

		float getAngle() const {
			return angle;
		}
		void setAngle(float _angle){
			angle = _angle;
		}

		float getAngularVelocity() const {
			return angular_velocity;
		}
		void setAngularVelocity(float _angular_velocity){
			angular_velocity = _angular_velocity;
		}

		const mat2x2& getTransform() const {
			if (transform_needs_update){
				transform = rotationMatrix(angle);
				transform_needs_update = false;
			}
			return transform;
		}

		const mat2x2& getInverseTransform() const {
			if (inv_transform_needs_update){
				inv_transform = rotationMatrix(-angle);
				inv_transform_needs_update = false;
			}
			return inv_transform;
		}

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



	struct Circle : RigidBody {
		Circle(float _radius, float density, float _elasticity)
			: RigidBody(
				RigidBody::Circle,
				(density * 2.0f * pi * _radius * _radius),
				(density * pi * _radius * _radius * _radius * _radius),
				_elasticity),
			radius(_radius) {

		}

		const float radius;

		BoundingBox getBoundingBox() const override {
			return BoundingBox(position.x - radius, position.y - radius, position.x + radius, position.y + radius);
		}

	};



	struct Rectangle : RigidBody {
		Rectangle(vec2 _size, float density, float _elasticity)
			: RigidBody(
				RigidBody::Rectangle,
				(density * _size.x * _size.y),
				(density * _size.x * _size.y * (_size.x * _size.x + _size.y * _size.y) / 12.0f),
				_elasticity),
			size(_size) {

		}

		const vec2 size;

		BoundingBox getBoundingBox() const override {

			vec2 topleft = getTransform() * (-size * 0.5f);
			vec2 bottomleft = getTransform() * (vec2(-size.x, size.y) * 0.5f);
			vec2 topright = getTransform() * (vec2(-size.x, size.y) * 0.5f);
			vec2 bottomright = getTransform() * (size * 0.5f);

			vec2 min = {
				std::min<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
				std::min<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
			};

			vec2 max = {
				std::max<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
				std::max<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
			};

			return BoundingBox(position + min, position + max);
		}
	};

} // namespace phys