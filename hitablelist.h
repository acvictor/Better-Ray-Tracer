#ifndef HITABLELISTH
#define HITABLELISTH

#include "sphere.h"
#include "movingSphere.h"

class hitableList: public hitable {
public:
	hitableList() {}
	hitableList(hitable** l, int n) { list = l; listSize = n; }
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const;

	hitable** list;
	int listSize;
};

bool hitableList::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
	hitRecord tempRec;
	bool hitAnything = 0;
	double closestSoFar = tMax;
	for (int i = 0; i < listSize; i++) {
		if(list[i]->hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = 1;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}

#endif