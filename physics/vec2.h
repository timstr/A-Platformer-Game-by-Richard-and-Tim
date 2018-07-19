#pragma once

#include "SFML/Graphics.hpp"

typedef sf::Vector2f vec2;

struct mat2x2 {
	mat2x2() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {

	}
	mat2x2(float topleft, float topright, float bottomleft, float bottomright)
		:a(topleft), b(topright), c(bottomleft), d(bottomright) {

	}
	mat2x2(const vec2& l, const vec2& r)
		: a(l.x), b(r.x), c(l.y), d(r.y) {

	}

	float a, b, c, d;
};

mat2x2 rotationMatrix(float radians);

mat2x2 operator*(const mat2x2& l, const mat2x2& r);

vec2 operator*(const mat2x2& m, const vec2& v);

const float pi = 3.1415927f;

float dot(const vec2& v1, const vec2& v2);

float cross(const vec2& v1, const vec2& v2);

vec2 projectOnto(const vec2& v1, const vec2& v2);

float abs(const vec2& v);