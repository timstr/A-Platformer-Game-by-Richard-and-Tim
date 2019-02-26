#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct CircleBody : RigidBody {
		CircleBody(float radius, float density, float elasticity);

		float radius() const;

		BoundingBox getBoundingBox() const override;

		bool hit(vec2 point_world_space) const override;

	private:

		const float m_radius;

	};

} // namespace phys