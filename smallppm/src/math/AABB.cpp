#include "AABB.h"
#include "numeric/NumericUtils.h"

bool AABB::Intersect(const Ray& ray, real* hitt0, real* hitt1) const {
	real t0 = 0, t1 = ray.tMax;
	for (int i = 0; i < 3; ++i) {
		// Update interval for _i_th bounding box slab
		real invRayDir = 1 / ray.d[i];
		real tNear = (minPoint[i] - ray.o[i]) * invRayDir;
		real tFar = (maxPoint[i] - ray.o[i]) * invRayDir;

		// Update parametric interval from slab intersection $t$ values
		if (tNear > tFar) std::swap(tNear, tFar);

		// Update _tFar_ to ensure robust ray--bounds intersection
		tFar *= 1 + 2 * gamma(3);
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	if (hitt0) *hitt0 = t0;
	if (hitt1) *hitt1 = t1;
	return true;
}


AABB Union(const AABB& a, const AABB& b) {

	Vec3 minPoint(
		std::min(a.minPoint.x, b.minPoint.x),
		std::min(a.minPoint.y, b.minPoint.y),
		std::min(a.minPoint.z, b.minPoint.z)
	);

	Vec3 maxPoint(
		std::max(a.maxPoint.x, b.maxPoint.x),
		std::max(a.maxPoint.y, b.maxPoint.y),
		std::max(a.maxPoint.z, b.maxPoint.z)
	);

	return AABB(minPoint, maxPoint);
}

AABB Union(const AABB& a, const Vec3& p) {
	AABB ret = a;
	ret.minPoint = Vec3(
		std::min(ret.minPoint.x, p.x),
		std::min(ret.minPoint.y, p.y),
		std::min(ret.minPoint.z, p.z)
	);
	ret.maxPoint = Vec3(
		std::max(ret.maxPoint.x, p.x),
		std::max(ret.maxPoint.y, p.y),
		std::max(ret.maxPoint.z, p.z)
	);
	return ret;
}