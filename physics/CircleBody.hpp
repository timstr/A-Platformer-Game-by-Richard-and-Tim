#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct CircleBody : RigidBody {
		CircleBody(float radius, float density, float elasticity);

		float radius() const;

		BoundingBox getBoundingBox() const override;

	private:

		const float m_radius;

	};

} // namespace phys