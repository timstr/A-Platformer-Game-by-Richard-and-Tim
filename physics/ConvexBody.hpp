#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct ConvexBody : RigidBody {

		// _points is a vector of points which define the perimeter of a 2D convex shape,
		// in a clockwise order
		// throws if the points are not such a convex shape
		ConvexBody(std::vector<vec2> points, float density, float elasticity);

		BoundingBox getBoundingBox() const override;

		bool hit(vec2 point_world_space) const override;

		const std::vector<vec2>& getPoints() const;

	private:

		// move points' center of mass to the origin
		static void normalize(std::vector<vec2>& points);

		// returns true if points is a set of points that defines a convex
		// shape's perimeter, in a clockwise ordering
		static bool isConvex(const std::vector<vec2>& points);

		static float getMass(const std::vector<vec2>& points, float density);

		static float getMoment(const std::vector<vec2>& points, float density);

		const std::vector<vec2> m_points;
	};

} // namespace phys