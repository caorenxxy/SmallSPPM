#pragma once
#include "Integrator.h"



GYT_NAMESPACE_BEGIN

class Sampler;
class SamplerEnum;
class StateSequence;

struct Tile {
	int minX, minY, maxX, maxY;
	Tile() = default;
	Tile(int minX, int minY, int maxX, int maxY): minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}
};


class TiledIntegrator : public Integrator
{
protected:
	int spp;
	std::shared_ptr<Sampler> mpSampler;
	std::shared_ptr<SamplerEnum> mpSamplerEnum;

public:
	TiledIntegrator() = default;
	TiledIntegrator(int samplePerPixel, const std::shared_ptr<Sampler> &pSampler, const std::shared_ptr<SamplerEnum> &pSamplerEnum): 
		spp(samplePerPixel), mpSampler(pSampler), mpSamplerEnum(pSamplerEnum) 
	{
	}
	virtual ~TiledIntegrator(){}
	virtual void Render(const Scene& scene, const Camera& camera) override;
protected:
	virtual Vec3 Li(const Ray &ray, const Scene& scene, StateSequence &rand, MemoryPool &arena) const = 0;
};

GYT_NAMESPACE_END
