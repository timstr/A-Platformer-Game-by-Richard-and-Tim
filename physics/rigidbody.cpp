#include "RigidBody.hpp"

namespace phys {

	RigidBody::RigidBody(Type _type, float _mass, float _moment, float _elasticity) :
		type(_type),
		velocity({0.0f, 0.0f}),
		position({0.0f, 0.0f}),
		forces({0.0f, 0.0f}),
		impulses({0.0f, 0.0f}),
		angle(0.0f),
		angular_velocity(0.0f),
		torques(0.0f),
		angular_impulses(0.0f),
		mass(_mass),
		inverse_mass(_mass == 0.0f ? 0.0f : (1.0f / _mass)),
		moment(_moment),
		inverse_moment(_moment == 0.0f ? 0.0f : (1.0f / _moment)),
		elasticity(_elasticity),
		transform_needs_update(true),
		inv_transform_needs_update(true) {

	}

	RigidBody::~RigidBody() {

	}

	void RigidBody::applyForceAt(vec2 force, vec2 point_world_space){
		applyForce(force);
		vec2 rad = point_world_space - position;
		// TODO: I'm pretty sure this is just the cross product of
		// point_world_space - position and force
		// Simplify using cross()
		applyTorque(rad.x * force.y - rad.y * force.x);
	}

	void RigidBody::applyForce(vec2 force){
		forces += force;
	}

	void RigidBody::applyTorque(float torque){
		torques += torque;
	}

	void RigidBody::applyImpulseAt(vec2 impulse, vec2 point_world_space){
		applyImpulse(impulse);
		vec2 rad = point_world_space - position;
		// TODO: I'm pretty sure this is just the cross product of
		// point_world_space - position and impulse
		// Simplify using cross()
		applyAngularImpulse(rad.x * impulse.y - rad.y * impulse.x);
	}

	void RigidBody::applyImpulse(vec2 impulse){
		impulses += impulse;
	}

	void RigidBody::applyAngularImpulse(float impulse){
		angular_impulses += impulse;
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
	void RigidBody::setVelocity(vec2 _velocity){
		velocity = _velocity;
	}

	float RigidBody::getAngle() const {
		return angle;
	}
	void RigidBody::setAngle(float _angle){
		angle = _angle;
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

	vec2 RigidBody::getEffectiveForces(float dt) const {
		return forces + impulses / dt;
	}

	float RigidBody::getEffectiveTorques(float dt) const {
		return torques + angular_impulses / dt;
	}

	void RigidBody::resetAccumulators(){
		forces = {0.0f, 0.0f};
		impulses = {0.0f, 0.0f};
		torques = 0.0f;
		angular_impulses = 0.0f;
	}

} // namespace phys