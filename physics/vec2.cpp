#include "Vec2.hpp"

namespace {
	const float epsilon = 1e-6f;
}

mat2x2::mat2x2() :
	a(0.0f),
	b(0.0f),
	c(0.0f),
	d(0.0f) {

}
mat2x2::mat2x2(float topleft, float topright, float bottomleft, float bottomright) :
	a(topleft),
	b(topright),
	c(bottomleft),
	d(bottomright) {

}
mat2x2::mat2x2(const vec2& l, const vec2& r) :
	a(l.x),
	b(r.x),
	c(l.y),
	d(r.y) {

}

mat2x2 rotationMatrix(float radians) {
	float cosine = cos(radians);
	float sine = sin(radians);
	return mat2x2(cosine, -sine, sine, cosine);
}

mat2x2 operator*(const mat2x2& l, const mat2x2& r) {
	return mat2x2(
		l.a * r.a + l.b * r.c,
		l.a * r.b + l.b * r.d,
		l.c * r.a + l.d * r.c,
		l.c * r.b + l.d * r.d
	);
}

vec2 operator*(const mat2x2& m, const vec2& v) {
	return vec2(
		m.a * v.x + m.b * v.y,
		m.c * v.x + m.d * v.y
	);
}

float dot(const vec2& v1, const vec2& v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

float cross(const vec2& v1, const vec2& v2) {
	return (v1.x * v2.y - v1.y * v2.x);
}

vec2 projectOnto(const vec2& v1, const vec2& v2) {
	if (v2.x == 0 && v2.y == 0){
		return {0, 0};
	}

	return v2 * (float)(dot(v1, v2) / dot(v2, v2));
}

vec2 orthogonalClockwise(const vec2& v) {
	return {v.y, -v.x};
}

vec2 orthogonalCounterclockwise(const vec2& v) {
	return {-v.y, v.x};
}

vec2 unit(const vec2& v) {
	const float mag = abs(v);
	if (!std::isnormal(mag)){
		return {0.0f, 0.0f};
	}
	return v / mag;
}

float distanceFromLinePP(const vec2& p1, const vec2& p2, const vec2& q) {
	return distanceFromLinePN(p1, orthogonalClockwise(p2 - p1), q);
}

float distanceFromLinePN(const vec2& p, const vec2& n, const vec2& q) {
	return abs(dot(q - p, n) / abs(n));
}

float displacementFromLinePN(const vec2& p, const vec2& n, const vec2& q) {
	return dot(q - p, n) / abs(n);
}

std::optional<vec2> lineSegmentIntersection(const vec2& p0, const vec2& p1, const vec2& q0, const vec2& q1){
	const vec2& p = p0;
	const vec2& q = q0;
	const vec2 r = p1 - p0;
	const vec2 s = q1 - q0;

	const vec2 q_minus_p = q - p;
	const float r_cross_s = cross(r, s);
	const float q_minus_p_cross_r = cross(q_minus_p, r);

	if (abs(r_cross_s) < epsilon){
		if (abs(q_minus_p_cross_r) < epsilon){
			// the two line segments are colinear
			const float r_dot_r = dot(r, r);
			const float t0 = dot(q_minus_p, r) / r_dot_r;
			const float t1 = t0 + dot(s, r) / r_dot_r;
			if (t0 >= 0.0f && t0 <= 1.0f && t1 >= 0.0f && t0 <= 0.0f){
				// line segments overlap
				return p + t0 * s;
			} else {
				// line segments do not overlap
				return {};
			}
		} else {
			return {};
		}
	} else {
		const float t = cross(q_minus_p, s) / r_cross_s;
		const float u = q_minus_p_cross_r / r_cross_s;
		if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f){
			return p + t * r;
		} else {
			return {};
		}
	}
}

float abs(const vec2& v){
	return hypot(v.x, v.y);
}