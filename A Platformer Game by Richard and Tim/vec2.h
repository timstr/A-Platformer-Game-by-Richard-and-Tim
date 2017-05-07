#pragma once

#include "SFML\Graphics.hpp"

typedef sf::Vector2f vec2;

const double pi = 3.14159265358979323846264338327950288;

double dot(const vec2& v1, const vec2& v2);

double cross(const vec2& v1, const vec2& v2);

vec2 projectOnto(const vec2& v1, const vec2& v2);

double abs(const vec2& v);