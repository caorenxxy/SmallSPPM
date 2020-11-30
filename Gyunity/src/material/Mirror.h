#pragma once
#include "common/Core.h"
#include "Material.h"
#include "texture/Texture.h"
#include "bsdf/BSDF.h"
GYT_NAMESPACE_BEGIN

class MirrorMaterial : public Material {
public:
	MirrorMaterial(const std::shared_ptr<Texture<Vec3>>& kr) : kr(kr) {

	}
	//void ComputeScatteringFunction(Intersection* isect,
	//	TransportMode mode = TransportMode::Radiance) const {

	//	isect->bsdf = std::shared_ptr<BSDF>(new SpecularBSDF(*isect, kr->Sample(*isect)));
	//}
	//void ComputeScatteringFunction(Intersection* isect,
	//	TransportMode mode = TransportMode::Radiance) const {

	//	isect->bsdf = std::shared_ptr<BSDF>(new BSDF(*isect));
	//	std::shared_ptr<BxDF> specularBSDF(new SpecularBSDF(kr->Sample(*isect)));
	//	isect->bsdf->Add(specularBSDF);
	//}

	void ComputeScatteringFunction(Intersection* isect, MemoryPool &arena,
		TransportMode mode = TransportMode::Radiance) const {

		isect->bsdf = MEMORY_POOL_ALLOC(arena, BSDF)(*isect);

		isect->bsdf->Add(MEMORY_POOL_ALLOC(arena, SpecularBSDF)(kr->Sample(*isect)));
	}
private:
	std::shared_ptr<Texture<Vec3>> kr;
};

GYT_NAMESPACE_END