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

	// TODO: add 
	
	struct RigidBody {

		enum Type {
			Rectangle,
			Circle
		};

		RigidBody(Type _type, float _mass, float _moment, float _elasticity) :
			type(_type),
			mass(_mass),
			inv_mass(_mass == 0.0f ? 0.0f : (1.0f / _mass)),
			moment(_moment),
			inv_moment(_moment == 0.0f ? 0.0f : (1.0f / _moment)),
			elasticity(_elasticity) {

		}
		virtual ~RigidBody(){

		}

		const Type type;

		const float elasticity;

		vec2 position;
		vec2 velocity;
		vec2 forces;

		float angle;
		float angular_velocity;
		float torques;

		const float mass;
		const float inv_mass;

		const float moment;
		const float inv_moment;

		mat2x2 transform;
		
		void applyForce(vec2 force, vec2 point_from_com){
			
		}

		void updateTransform(){
			transform = rotationMatrix(angle);
		}

		virtual BoundingBox getBoundingBox() const = 0;
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
			return BoundingBox(position - size * 0.5f, position + size * 0.5f);
		}
	};

} // namespace phys