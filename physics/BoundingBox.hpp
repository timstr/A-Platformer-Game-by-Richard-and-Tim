#pragma once

#include "Vec2.hpp"

namespace phys {

	struct BoundingBox {
		BoundingBox(vec2 _min, vec2 _max);
		BoundingBox(float minx, float miny, float maxx, float maxy);

		bool collidesWith(const BoundingBox& other) const;

		vec2 min;
		vec2 max;
	};

} // namespace phys