#include "Constraint.hpp"

phys::CollisionConstraint::CollisionConstraint(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b) :
    a(_a),
    b(_b),
    normal(_normal),
    radius_a(_radius_a),
    radius_b(_radius_b),
    impulse({0.0f, 0.0f}) {

}

void phys::CollisionConstraint::solve(float dt) {
    const vec2 old_impulse = impulse;
    vec2 delta = computeImpulse(dt);
    impulse += delta;
    impulse = clampToNormal(impulse, normal);
    delta = impulse - old_impulse;
    a.applyImpulseAt(-delta, a.getPosition() + radius_a);
    b.applyImpulseAt(delta, b.getPosition() + radius_b);

	/*const vec2 delta = clampToNormal(computeImpulse(), normal);
	a.applyImpulseAt(-delta, a.getPosition() + radius_a);
	b.applyImpulseAt(delta, b.getPosition() + radius_b);*/
}

vec2 phys::CollisionConstraint::computeImpulse(float dt) const {
    //const float depth = dot(a.getPosition() + radius_a - b.getPosition() - radius_b, normal);
    //const float depth_correction = 0.2f;

    const float restitution = std::min(a.elasticity, b.elasticity);
    const float term_a = pow(cross(radius_a, normal), 2.0f) * a.inverse_moment;
    const float term_b = pow(cross(radius_b, normal), 2.0f) * b.inverse_moment;
    const vec2 v_a = a.getVelocity() + a.getAngularVelocity() * orthogonalCW(radius_a);
    const vec2 v_b = b.getVelocity() + b.getAngularVelocity() * orthogonalCW(radius_b);
    const vec2 v_rel = v_b - v_a;
    const float v_norm = dot(v_rel, normal);
	const float denom = a.inverse_mass + b.inverse_mass + term_a + term_b;
	if (abs(denom) < 1e-6f){
		return {0.0f, 0.0f};
	}
	return -(1.0f + restitution) * v_norm / denom * normal;
}

phys::GlueConstraint::GlueConstraint(RigidBody& _a, RigidBody& _b, vec2 _displacement, float _angle) :
    a(_a),
    b(_b),
    displacement(_displacement),
    angle(_angle) {

}

void phys::GlueConstraint::solve(float dt) {
    // TODO
}

phys::DistanceJointConstraint::DistanceJointConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b, float _distance) :
    a(_a),
    b(_b),
    radius_a(_radius_a),
    radius_b(_radius_b),
    distance(_distance) {

}

void phys::DistanceJointConstraint::solve(float dt) {
    // TODO
}

phys::RevoluteJointConstraint::RevoluteJointConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b) :
    a(_a),
    b(_b),
    radius_a(_radius_a),
    radius_b(_radius_b) {

}

void phys::RevoluteJointConstraint::solve(float dt) {
    // TODO
}

