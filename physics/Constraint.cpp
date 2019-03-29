#include "Constraint.hpp"

namespace phys {

	CollisionConstraint::CollisionConstraint(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b, float _depth) :
		a(_a),
		b(_b),
		normal(_normal),
		radius_a(_radius_a),
		radius_b(_radius_b),
		depth(_depth),
		impulse(0.0f) {
	}

	void CollisionConstraint::solve(float dt) {
		const float old_impulse = impulse;
		float delta = computeImpulse(dt);
		impulse += delta;
		impulse = std::max(0.0f, impulse);
		delta = impulse - old_impulse;

		a.applyImpulseAt(-delta * normal, radius_a);
		b.applyImpulseAt(delta * normal, radius_b);
	}

	float CollisionConstraint::computeImpulse(float dt) const {
		const float corr = velocity_steering * depth * dt;

		const float restitution = std::min(a.elasticity, b.elasticity);
		const vec2 v_a = a.getVelocityAt(radius_a);
		const vec2 v_b = b.getVelocityAt(radius_b);
		const vec2 v_rel = v_b - v_a;
		const float v_norm = dot(v_rel, normal);
		const float invmass_a = a.getApparentInverseMassAt(radius_a, normal);
		const float invmass_b = b.getApparentInverseMassAt(radius_b, normal);
		const float denom = invmass_a + invmass_b;
		if (abs(denom) < 1e-6f){
			return 0.0f;
		}
		return (-(1.0f + restitution) * v_norm + corr) / denom;
	}

	GlueConstraint::GlueConstraint(RigidBody& _a, RigidBody& _b, vec2 _displacement, float _angle) :
		a(_a),
		b(_b),
		displacement(_displacement),
		angle(_angle) {

	}

	void GlueConstraint::solve(float dt) {
		/*{
			vec2 mid = (a.getPosition() * a.mass + b.getPosition() * b.mass) / (a.mass + b.mass);

			//vec2 v_rel_a = a.getVelocityAtW(b.getPosition()) - b.getVelocity();
			//vec2 v_rel_b = b.getVelocityAtW(a.getPosition()) - a.getVelocity();
			vec2 v_rel_a = a.getVelocityAtW(mid);
			vec2 v_rel_b = b.getVelocityAtW(mid);

			vec2 v_rel = v_rel_b - v_rel_a;

			float inv_mass_a = a.getApparentInverseMassAtW(mid, v_rel);
			float inv_mass_b = b.getApparentInverseMassAtW(mid, v_rel);
			float denom = inv_mass_a + inv_mass_b;

			vec2 impulse = -v_rel / denom;
			//a.applyImpulseAtW(-impulse, b.getPosition());
			//b.applyImpulseAtW(impulse, a.getPosition());
			a.applyImpulseAtW(-impulse, mid);
			b.applyImpulseAtW(impulse, mid);
		}*/
		{
			vec2 v_rel_a = a.getVelocityAtW(b.getPosition()) - b.getVelocity();
			vec2 v_rel_b = b.getVelocityAtW(a.getPosition()) - a.getVelocity();
			vec2 v_rel = v_rel_b - v_rel_a;
			vec2 disp = b.getPosition() - a.getPosition();

			float inv_mass_a = a.getApparentInverseMassAtW(b.getPosition(), orthogonalCW(disp));
			float inv_mass_b = b.getApparentInverseMassAtW(a.getPosition(), orthogonalCW(disp));
			float denom = inv_mass_a + inv_mass_b;

			vec2 impulse = v_rel / denom;
			a.applyImpulseAtW(impulse, b.getPosition());
			b.applyImpulseAtW(-impulse, a.getPosition());
		}
		/*{
			vec2 v_rel_a = a.getVelocityAtW(b.getPosition()) - b.getVelocity();
			vec2 v_rel_b = b.getVelocityAtW(a.getPosition()) - a.getVelocity();

			float inv_mass_a = a.getApparentInverseMassAtW(b.getPosition(), v_rel_a);
			float inv_mass_b = b.getApparentInverseMassAtW(a.getPosition(), v_rel_b);

			a.applyImpulseAtW(-v_rel_a / inv_mass_a, b.getPosition());
			b.applyImpulseAtW(-v_rel_b / inv_mass_b, a.getPosition());
		}*/
	}


	HingeConstraint::HingeConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b, float _distance) :
		a(_a),
		b(_b),
		radius_a(_radius_a),
		radius_b(_radius_b),
		distance(_distance) {

	}

	void HingeConstraint::solve(float dt) {
		const vec2 r_a = a.getInverseTransform() * radius_a;
		const vec2 r_b = b.getInverseTransform() * radius_b;

		const vec2 disp = b.getPosition() + r_b - a.getPosition() - r_a;
		const float diff = abs(disp) - distance;
		const float corr = diff * velocity_steering * dt;

		const vec2 v_rel = b.getVelocityAt(r_b) - a.getVelocityAt(r_a);

		const vec2 normal = unit(disp);

		const float invmass_a = a.getApparentInverseMassAt(r_a, normal);
		const float invmass_b = b.getApparentInverseMassAt(r_b, normal);
		const float denom = invmass_a + invmass_b;
		if (abs(denom) < 1e-6f){
			return;
		}
		const float impulse = (dot(v_rel, normal) + corr) / denom;
		a.applyImpulseAt(impulse * normal, r_a);
		b.applyImpulseAt(-impulse * normal, r_b);
	}

	RevoluteJointConstraint::RevoluteJointConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b) :
		a(_a),
		b(_b),
		radius_a(_radius_a),
		radius_b(_radius_b) {

	}

	void RevoluteJointConstraint::solve(float dt) {
		// TODO
	}

} // namespace phys