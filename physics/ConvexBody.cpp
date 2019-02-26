#include "ConvexBody.hpp"

namespace phys {

	ConvexBody::ConvexBody(std::vector<vec2> points, float density, float elasticity) :
		RigidBody(
			RigidBody::Convex,
			(normalize(points), getMass(points, density)),
			getMoment(points, density),
			elasticity
		),
		m_points(std::move(points)) {

	}

	BoundingBox ConvexBody::getBoundingBox() const {
		vec2 pmin = getInverseTransform() * m_points.front();
		vec2 pmax = pmin;
		for (int i = 1; i < m_points.size(); ++i){
			auto pt = getInverseTransform() * m_points[i];
			pmin = {std::min(pmin.x, pt.x), std::min(pmin.y, pt.y)};
			pmax = {std::max(pmax.x, pt.x), std::max(pmax.y, pt.y)};
		}
		return {pmin + getPosition(), pmax + getPosition()};
	}

	bool ConvexBody::hit(vec2 point_world_space) const {
		vec2 q = getTransform() * (point_world_space - getPosition());
		for (int i = 0; i < m_points.size(); ++i){
			const vec2& p0 = m_points[i];
			const vec2& p1 = m_points[(i + 1) % m_points.size()];
			const float dist = cross(p0 - p1, q - p1);
			if (dist > 0.0f) {
				return false;
			}
		}
		return true;
	}

	const std::vector<vec2>& ConvexBody::getPoints() const {
		return m_points;
	}

	void ConvexBody::normalize(std::vector<vec2>& points) {
		if (points.size() < 3){
			throw std::runtime_error("Not enough points to define convex shape");
		}
		if (!isConvex(points)){
			throw std::runtime_error("The points do not define a convex shape");
		}
		// average of all points, used to create triangle fan
		vec2 avg = {0.0f, 0.0f};
		for (const vec2& p : points){
			avg += p;
		}
		avg /= (float)points.size();

		// center of mass
		vec2 com = {0.0f, 0.0f};
		float total_area = 0.0f;

		// for every triangle from midpoint to neighbouring pair of points
		for (int i = 0; i < points.size(); ++i){
			const vec2& a = avg;
			const vec2& b = points[i];
			const vec2& c = points[(i + 1) % points.size()];
			const float area = 0.5f * cross(b - a, c - a);
			const vec2 centroid = (a + b + c) / 3.0f;

			com += centroid * area;
			total_area += area;
		}

		// move all points
		for (vec2& p : points){
			p -= com / total_area;
		}
	}

	bool ConvexBody::isConvex(const std::vector<vec2>& points) {
		// TODO: test
		for (int i = 0; i < points.size(); ++i){
			const vec2& p0 = points[i];
			const vec2& p1 = points[(i + 1) % points.size()];
			const vec2& p2 = points[(i + 2) % points.size()];
			const float dist = cross(p0 - p1, p2 - p1);
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
		float num = 0.0f;
		float den = 0.0f;
		float area = 0.0f;
		for (int i = 0; i < points.size(); ++i){
			const vec2& p0 = points[i];
			const vec2& p1 = points[(i + 1) % points.size()];

			area += 0.5f * cross(p0, p1);
			num += abs(cross(p1, p0)) * (dot(p0, p0) + dot(p0, p1) + dot(p1, p1));
			den += abs(cross(p1, p0));
		}
		return area * density * num / (6.0f * den);
	}

} // namespace phys