#ifndef TEXTUREH
#define TEXTUREH

#include "ray.h"

class texture {
public:
	vec3 value(float u, float v, const vec3& p) const;
};