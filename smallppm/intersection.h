#pragma once

#include "linagl.h"
#include "ray.h"
#include "geometry_util.h"
#include "material.h"

NAMESPACE_BEGIN

class BSDF;
class Primitive;

class Intersection {
public:
	Vec3 hit, n, nl, ng, wo;
	Vec3 dpdu, dpdv;
	Vec3 dpdus, dpdvs;
	Vec2 uv;
	real rayEps;
	Vec3 pError;
	real b1, b2;
	uint64 shapeId;
	const Primitive *primitive;
	//std::shared_ptr<BSDF> bsdf;
	BSDF* bsdf;

	Intersection() { rayEps = 0; shapeId = -1;}

	Intersection(const Vec3 &hit, const Vec3 &ng, const Vec3 &nl, const Vec3 &wo, const Vec3 &pError):
		hit(hit), ng(ng), nl(nl), wo(wo), pError(pError), rayEps(0), primitive(nullptr){
		bsdf = nullptr;
		shapeId = -1;
	}

	Intersection(const Vec3& hit, const Vec3& ng, const Vec3& nl, 
		const Vec3& dpdu, const Vec3& dpdv, const Vec3& wo, const Vec3& pError) :
		hit(hit), dpdu(dpdu), dpdv(dpdv), ng(ng), nl(nl), wo(wo), pError(pError), rayEps(0), primitive(nullptr) {
		bsdf = nullptr;
		shapeId = -1;
	}

	void SetShading(const Vec3& ns, const Vec3& dpdus, Vec3& dpdvs) {
		n = ns;
		this->dpdus = dpdus;
		this->dpdvs = dpdvs;
		//n = Cross(dpdus, dpdvs);
		//n = (Dot(n, ng) > 0) ? n : -n;
		//this->dpdus = dpdus;
		//this->dpdvs = dpdvs;
	}

	void ComputeScatteringFunction(MemoryArena &arena, TransportMode mode = TransportMode::Radiance);

	Ray SpawnTo(const Intersection &it) const {
		Vec3 origin = OffsetRayOrigin(hit, pError, nl, it.hit - hit);
		Vec3 target = OffsetRayOrigin(it.hit, it.pError, it.nl, origin - it.hit);
		Vec3 d = target - origin;
		real dis = d.Length();
		Normalize(d);
		return Ray(origin, d, dis - shadowRayEps, rayEps);
		//return Ray(origin, d, 1 - shadowRayEps, 0.f);
	}

	Ray SpawnRay(const Vec3 &d) const {
		Vec3 o = OffsetRayOrigin(hit, pError, nl, d);
		//Vec3 o = hit + d * 0.0001;
		return Ray(o, d, Infinity, rayEps);
	}

	//Ray SpawnTo(const Intersection &isect) const {
	//	Vec3 dir = isect.hit - (hit + nl * nEps);
	//	real d = dir.Length();
	//	dir.Normalize();
	//	//return Ray(hit + dir * rayeps + nl * rayeps, dir, d - shadowRayEps);
	//	return Ray(hit, dir, d * (1.f - isect.rayEps), rayEps);
	//}
};

NAMESPACE_END