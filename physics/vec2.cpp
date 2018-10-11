#include "Vec2.hpp"

mat2x2::mat2x2() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {

}
mat2x2::mat2x2(float topleft, float topright, float bottomleft, float bottomright)
	:a(topleft), b(topright), c(bottomleft), d(bottomright) {

}
mat2x2::mat2x2(const vec2& l, const vec2& r)
	: a(l.x), b(r.x), c(l.y), d(r.y) {

}

mat2x2 rotationMatrix(float radians){
	float cosine = cos(radians);
	float sine = sin(radians);
	return mat2x2(cosine, -sine, sine, cosine);
}

mat2x2 operator*(const mat2x2& l, const mat2x2& r){
	return mat2x2(
		l.a * r.a + l.b * r.c,
		l.a * r.b + l.b * r.d,
		l.c * r.a + l.d * r.c,
		l.c * r.b + l.d * r.d
	);
}

vec2 operator*(const mat2x2& m, const vec2& v){
	return vec2(
		m.a * v.x + m.b * v.y,
		m.c * v.x + m.d * v.y
	);
}

float dot(const vec2& v1, const vec2& v2){
	return (v1.x * v2.x + v1.y * v2.y);
}

float cross(const vec2& v1, const vec2& v2){
	return (v1.x * v2.y - v1.y * v2.x);
}

vec2 projectOnto(const vec2& v1, const vec2& v2){
	if (v2.x == 0 && v2.y == 0){
		return {0, 0};
	}

	return v2 * (float)(dot(v1, v2) / dot(v2, v2));
}

float abs(const vec2& v){
	return hypot(v.x, v.y);
}