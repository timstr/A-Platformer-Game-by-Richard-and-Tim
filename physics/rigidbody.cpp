#include "RigidBody.hpp"

namespace phys {

	RigidBody::RigidBody(Type _type, float _mass, float _moment, float _elasticity) :
		type(_type),
		velocity({0.0f, 0.0f}),
		position({0.0f, 0.0f}),
		angle(0.0f),
		angular_velocity(0.0f),
		mass(_mass),
		inverse_mass(_mass == 0.0f ? 0.0f : (1.0f / _mass)),
		moment(_moment),
		inverse_moment(_moment == 0.0f ? 0.0f : (1.0f / _moment)),
		elasticity(_elasticity),
		friction(0.5f),
		transform_needs_update(true),
		inv_transform_needs_update(true) {

	}

	RigidBody::~RigidBody() {

	}

	void RigidBody::setFriction(float _friction) {
		friction = _friction;
	}

	float RigidBody::getFriction() const {
		return friction;
	}

	void RigidBody::applyForceAt(vec2 force, vec2 point_world_space, float dt){
		applyForce(force, dt);
		applyTorque(cross(position - point_world_space, force), dt);
	}

	void RigidBody::applyForce(vec2 force, float dt){
		velocity += force * inverse_mass * dt;
	}

	void RigidBody::applyTorque(float torque, float dt){
		angular_velocity += torque * inverse_moment * dt;
	}

	void RigidBody::applyImpulseAt(vec2 impulse, vec2 point_world_space){
		applyImpulse(impulse);
		applyAngularImpulse(cross(position - point_world_space, impulse));
	}

	void RigidBody::applyImpulse(vec2 impulse){
		velocity += impulse * inverse_mass;
	}

	void RigidBody::applyAngularImpulse(float impulse){
		angular_velocity += impulse * inverse_moment;
	}

	const vec2& RigidBody::getPosition() const {
		return position;
	}
	void RigidBody::setPosition(vec2 _position){
		position = _position;
	}

	const vec2& RigidBody::getVelocity() const {
		return velocity;
	}

	const vec2 RigidBody::getVelocityAt(vec2 point_world_space) const {
		return getVelocity() + orthogonalCW(point_world_space - getPosition()) * getAngularVelocity();
	}

	void RigidBody::setVelocity(vec2 _velocity){
		velocity = _velocity;
	}

	float RigidBody::getAngle() const {
		return angle;
	}
	void RigidBody::setAngle(float _angle){
		angle = _angle;
		transform_needs_update = true;
		inv_transform_needs_update = true;
	}

	float RigidBody::getAngularVelocity() const {
		return angular_velocity;
	}
	void RigidBody::setAngularVelocity(float _angular_velocity){
		angular_velocity = _angular_velocity;
	}

	const mat2x2& RigidBody::getTransform() const {
		if (transform_needs_update){
			transform = rotationMatrix(angle);
			transform_needs_update = false;
		}
		return transform;
	}

	const mat2x2& RigidBody::getInverseTransform() const {
		if (inv_transform_needs_update){
			inv_transform = rotationMatrix(-angle);
			inv_transform_needs_update = false;
		}
		return inv_transform;
	}

} // namespace phys