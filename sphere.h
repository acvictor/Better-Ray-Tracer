#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r) : center(cen), radius(r) {};
	sphere(vec3 cen, float r, material* m) : center(cen), radius(r), matPtr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;

	vec3 center;
	float radius;
	material* matPtr;
};

bool sphere::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float D = b * b - a * c;

	if(D > 0) {
		float temp = (-b - sqrt(D)) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}

		temp = (-b + sqrt(D)) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}

	return false;
}

#endif