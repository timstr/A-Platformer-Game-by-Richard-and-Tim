#pragma once
#include "vec2.h"

double dot(const vec2& v1, const vec2& v2){
	return (v1.x * v2.x + v1.y * v2.y);
}

double cross(const vec2& v1, const vec2& v2){
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

void nanCheck(vec2& v){
	if (std::isnan(v.x + v.y)){
		v = {0, 0};
	}
}