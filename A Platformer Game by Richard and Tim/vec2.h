#pragma once

#include "SFML\Graphics.hpp"

typedef sf::Vector2f vec2;

struct mat2x2 {
	mat2x2(float topleft = 0, float topright = 0, float bottomleft = 0, float bottomright = 0){
		a = topleft;
		b = topright;
		c = bottomleft;
		d = bottomright;
	}
	mat2x2(const vec2& l, const vec2& r){
		a = l.x;
		b = r.x;
		c = l.y;
		d = r.y;
	}

	float a, b, c, d;
};

mat2x2 rotationMatrix(double radians);

mat2x2 operator*(const mat2x2& l, const mat2x2& r);

vec2 operator*(const mat2x2& m, const vec2& v);

vec2 operator*(const vec2& l, const vec2& r);

const double pi = 3.14159265358979323846264338327950288;

double dot(const vec2& v1, const vec2& v2);

double cross(const vec2& v1, const vec2& v2);

vec2 projectOnto(const vec2& v1, const vec2& v2);

double abs(const vec2& v);