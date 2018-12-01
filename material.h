#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"

vec3 randomInUnitSphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
	} while(p.squaredLength() >= 1,0);
	return p;
}

float schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 *= r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted) {
	vec3 uv= unitVector(v);
	float dt = dot(uv, n);
	float D = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
	if(D > 0) {
		refracted = niOverNt * (uv - n * dt) - n * sqrt(D);
		return 1;
	}
	else
		return 0;
}

class material {
public:
	virtual bool scatter(const ray& rIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian: public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& rIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

class metal: public material {
public:
	metal(const vec3& a, float f) : albedo(a) { if(f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& rIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * randomInUnitSphere(), rIn.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material {
public:
	dielectric(float ri) : refIdx(ri) {}
	virtual bool scatter(const ray& rIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
		vec3 outwardNormal;
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		attenuation = vec3(1, 1, 1);
		vec3 refracted;
		float reflectProb;
		float cosine;

		if(dot(rIn.direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}

		if(refract(rIn.direction(), outwardNormal, niOverNt, refracted)) {
			reflectProb = schlick(cosine, refIdx);
		}
		else {
			reflectProb = 1.0;
		}

		if(drand48() < reflectProb) {
			scattered = ray(rec.p, reflected, rIn.time());
		}
		else {
			scattered = ray(rec.p, refracted, rIn.time());
		}

		return true;
	}

	float refIdx;
};

#endif