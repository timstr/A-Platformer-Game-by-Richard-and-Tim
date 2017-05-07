#pragma once

#include "SFML\Graphics.hpp"

typedef sf::Vector2f vec2;

double dot(const vec2& v1, const vec2& v2);

double cross(const vec2& v1, const vec2& v2);

vec2 projectOnto(const vec2& v1, const vec2& v2);