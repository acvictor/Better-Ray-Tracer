#ifndef MOVINGSPHEREH
#define MOVINGSPHEREH

#include "hitable.h"

class movingSphere : public hitable {
public:
	movingSphere() {}
	movingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const;
	vec3 center(float time) const;

	vec3 center0, center1;
	float time0, time1;
	float radius;
	material* matPtr;
};

vec3 movingSphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool movingSphere::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float D = b * b - a * c;

	if(D > 0) {
		float temp = (-b - sqrt(D)) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.matPtr = matPtr;
			return true;
		}

		temp = (-b + sqrt(D)) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}

	return false;
}

#endif
