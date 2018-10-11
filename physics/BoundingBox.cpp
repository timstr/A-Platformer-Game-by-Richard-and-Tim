#include "BoundingBox.hpp"

namespace phys {

	BoundingBox::BoundingBox(vec2 _min, vec2 _max) : min(_min), max(_max) {

	}
	BoundingBox::BoundingBox(float minx, float miny, float maxx, float maxy) : min(minx, miny), max(maxx, maxy) {

	}

	bool BoundingBox::collidesWith(const BoundingBox& other) const {
		return
			max.x > other.min.x &&
			max.y > other.min.y &&
			min.x < other.max.x &&
			min.y < other.max.y;
	}

} // namespace phys