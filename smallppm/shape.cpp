#include "shape.h"
#include "intersection.h"

NAMESPACE_BEGIN

Intersection Shape::Sample(const Intersection& isect, real* pdf, const Vec2& u) const {
	Intersection it = Sample(pdf, u);
	Vec3 wi = isect.hit - it.hit;
	if (wi.Length() == 0) {
		*pdf = 0;
	}
	else {
		wi.Normalize();
		*pdf *= Distance2(isect.hit, it.hit) / std::abs(Dot(it.n, -wi));
		if (std::isinf(*pdf)) *pdf = 0;
	}
	return it;
}

NAMESPACE_END