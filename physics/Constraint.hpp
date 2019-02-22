#pragma once

#include "RigidBody.hpp"

namespace phys {

	/*
    Every constraint has a position equation which describes how far away from the desired
    state the system is. This equation depends on the positions of a number of rigid bodies,
    and should be zero if and only if the constraint is satisfied.

    The first derivative with respect to time of this equation gives the velocity constraint

    */


	struct Constraint {
        virtual void solve(float dt) = 0;
	};

    //-----------------------------------------

	struct CollisionConstraint : Constraint {
        CollisionConstraint(RigidBody& _a, RigidBody& _b, vec2 _normal, vec2 _radius_a, vec2 _radius_b, float depth);

        void solve(float dt) override;

        float computeImpulse(float dt) const;

        RigidBody& a;
        RigidBody& b;
        const vec2 normal; // normal at point of collision, pointing from a to b
        const vec2 radius_a; // vector from a's center of mass to point of collision (closest point to b), in world coordinates
        const vec2 radius_b; // vector from b's center of mass to point of collision (closest point to a), in world coordinates
		const float depth; // penetration depth

        float impulse; // accumulated impulse over the course of many iterations
	};

	// constrains two bodies to have zero relative motion
	struct GlueConstraint : Constraint {
		GlueConstraint(RigidBody& _a, RigidBody& _b, vec2 _displacement, float _angle);

        void solve(float dt) override;

        RigidBody& a;
        RigidBody& b;
        const vec2 displacement; // offset from a to b in a's coordinates
        const float angle; // difference in angle
	};

    // TODO: glue constraint with arbitrary number of bodies fixed to eachother, for larger structures?

	// constrains two points on two bodies to be a fixed
	// distance apart
	struct DistanceJointConstraint : Constraint {
		DistanceJointConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b, float _distance);

        void solve(float dt) override;
        
        RigidBody& a;
        RigidBody& b;
        const vec2 radius_a;
        const vec2 radius_b;
        const float distance;
	};

	// constrains two bodies to not move relative to a fixed
	// point on both bodies while allowing free rotation
	struct RevoluteJointConstraint : Constraint {
		// TODO: motor? spring?

        RevoluteJointConstraint(RigidBody& _a, RigidBody& _b, vec2 _radius_a, vec2 _radius_b);

        void solve(float dt) override;
        
        RigidBody& a;
        RigidBody& b;
        const vec2 radius_a;
        const vec2 radius_b;
	};

	// TODO: add other types of constraints

} // namespace phys