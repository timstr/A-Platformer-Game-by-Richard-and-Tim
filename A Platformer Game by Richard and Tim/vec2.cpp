#pragma once
#include "vec2.h"

mat2x2 rotationMatrix(double radians){
	double cosine = cos(radians);
	double sine = sin(radians);
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

double abs(const vec2& v){
	return hypot(v.x, v.y);
}