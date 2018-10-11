#include "ConvexBody.hpp"

namespace phys {

	ConvexBody::ConvexBody(std::vector<vec2> points, float density, float elasticity) :
		RigidBody(
			RigidBody::Convex,
			getMass(points, density),
			getMoment(points, density),
			elasticity
		),
		m_points(std::move(points)) {

	}

	BoundingBox ConvexBody::getBoundingBox() const {
		vec2 pmin = m_points.front();
		vec2 pmax = pmin;
		for (const vec2& p : m_points){
			auto pt = getTransform() * p;
			pmin = {std::min(pmin.x, pt.x), std::min(pmin.y, pt.y)};
			pmax = {std::max(pmax.x, pt.x), std::max(pmax.y, pt.y)};
		}
		return {pmin, pmax};
	}

	void ConvexBody::normalize(std::vector<vec2>& points) {
		// TODO: test

		if (points.size() < 3){
			throw std::runtime_error("Not enough points to define convex shape");
		}
		// average of all points, used to create triangle fan
		vec2 avg = {0, 0};
		for (const vec2& p : points){
			avg += p;
		}
		avg /= (float)points.size();

		// center of mass
		vec2 com = {0, 0};

		// for every triangle from midpoint to neighbouring pair of points
		for (int i = 0; i < points.size(); ++i){
			const vec2& a = avg;
			const vec2& b = points[i];
			const vec2& c = points[(i + 1) % points.size()];
			const float area = 0.5f * cross(b - a, c - a);
			const vec2 centroid = (a + b + c) / 3.0f;

			com += centroid * area;
		}

		// move all points
		for (vec2& p : points){
			p -= com;
		}
	}

	bool ConvexBody::isConvex(const std::vector<vec2>& points) {
		// TODO: test
		for (int i = 0; i < points.size(); ++i){
			const vec2& p0 = points[i];
			const vec2& p1 = points[(i + 1) % points.size()];
			const vec2& p2 = points[(i + 2) % points.size()];
			const float dist = (p2.y - p1.y) * p0.x - (p2.x - p1.x) * p0.y + p2.x * p1.y - p2.y * p1.x;
			if (dist > 0.0f) {
				return false;
			}
		}
		return true;
	}

	float ConvexBody::getMass(const std::vector<vec2>& points, float density) {
		float area = 0;
		for (int i = 0; i < points.size(); ++i){
			const vec2& b = points[i];
			const vec2& c = points[(i + 1) % points.size()];
			area += 0.5f * cross(b, c);
		}
		return area * density;
	}

	float ConvexBody::getMoment(const std::vector<vec2>& points, float density) {
		float moment = 0;
		for (int i = 0; i < points.size(); ++i){
			const vec2& b = points[i];
			const vec2& c = points[(i + 1) % points.size()];
			const float area = 0.5f * cross(b, c);
			const vec2 centroid = (b + c) / 3.0f;
			moment += area * exp2f(abs(centroid));
		}
		return moment * density;
	}

} // namespace phys