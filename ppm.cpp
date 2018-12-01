#include <bits/stdc++.h>
#include "hitablelist.h"
#include "camera.h"
#include "material.h"

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define inf 1000000

hitable* randomScene() {
	int n = 500;
	hitable** list = new hitable * [n+1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float chooseMat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
			if ((center - vec3(4, .2, 0)).length() > 0.9) {
				if(chooseMat < 0.8) {
					list[i++] = new movingSphere(center, center+vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if(chooseMat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(.4, 0.2, 0.2)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitableList(list, i);
}

vec3 color(const ray& r, hitable* world, int depth) {
	hitRecord rec;
	if(world->hit(r, 0.001, numeric_limits<float>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if(depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	/* 
	 * unit vector between -1, 1
	 * +1 to shift between 0, 2
	 * *.5 to scale to between 0, 1
	 */ 
	else {
		vec3 unitDirection = unitVector(r.direction()); 
		float t = 0.5 * (unitDirection.y() + 1.0);
		/*
		 * linear interpolation
		 * 0 < t < 1
		 */
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main(void) {
	_
	ofstream f;
	f.open("sample.ppm");

	int nx = 900, ny = 600, ns = 10;
	f << "P3\n" << nx << " " << ny <<"\n255\n";

	hitable *list[5];
    float R = cos(M_PI/4);
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
	hitable *world = new hitableList(list,5);

	world = randomScene();

	vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float distToFocus = 10.0;
    float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, distToFocus, 0, 1);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			vec3 col(0, 0, 0);
			for(int s = 0; s < ns; s++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.getRay(u, v);
				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world, 0);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
 
			f << (int)(255.99 * col[0]) << " " << (int)(255.99 * col[1]) << " " << (int)(255.99 * col[2]) << endl;
		}
	}
	f.close();
	return 0;
}

