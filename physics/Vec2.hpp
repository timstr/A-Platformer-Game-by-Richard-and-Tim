#pragma once

#include "SFML/Graphics.hpp"

typedef sf::Vector2f vec2;

struct mat2x2 {
	mat2x2();
	mat2x2(float topleft, float topright, float bottomleft, float bottomright);
	mat2x2(const vec2& l, const vec2& r);

	float a, b, c, d;
};

mat2x2 rotationMatrix(float radians);

mat2x2 operator*(const mat2x2& l, const mat2x2& r);

vec2 operator*(const mat2x2& m, const vec2& v);

const float pi = 3.1415927f;

float dot(const vec2& v1, const vec2& v2);

float cross(const vec2& v1, const vec2& v2);

vec2 projectOnto(const vec2& v1, const vec2& v2);

vec2 orthogonalClockwise(const vec2& v);

vec2 orthogonalCounterclockwise(const vec2& v);

vec2 unit(const vec2& v);

// absolute distance between a point q and a line that passes through p1 and p2
float distanceFromLinePP(const vec2& p1, const vec2& p2, const vec2& q);

// absolute distance between a point q and a line that passes through p and is orthogonal to n
float distanceFromLinePN(const vec2& p, const vec2& n, const vec2& q);

// displacement between a point q and a line that passes through p and is orthogonal to n
// displacement is negative if q is on opposite side of line from n
float displacementFromLinePN(const vec2& p, const vec2& n, const vec2& q);

float abs(const vec2& v);