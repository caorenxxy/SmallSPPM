#include "homogeneous.h"
#include <algorithm>
#include "intersection.h"
#include "scene.h"

NAMESPACE_BEGIN

Vec3 HomogeneousMedium::Tr(const Ray& ray, StateSequence& rand) const {
    return Exp(-sigma_t * std::min(ray.tMax * ray.d.Length(), MaxReal));
}


Vec3 HomogeneousMedium::Sample(const Ray& ray, StateSequence& rand, MemoryArena& arena, MediumIntersection* mi) const {
    // Sample a channel and distance along the ray
    int channel = std::min((int)(rand() * 3), 2);
    //int channel = 0;
    real dist = -std::log(1 - rand()) / sigma_t[channel];
    //real dist = -std::log(1 - std::max(0.f, rand() - 0.1f)) / sigma_t[channel];
    real t = std::min(dist / ray.d.Length(), ray.tMax);
    bool sampledMedium = t < ray.tMax;
    if (sampledMedium)
        *mi = MediumIntersection(ray(t), -ray.d, this, ARENA_ALLOC(arena, HenyeyGreenstein)(g));

    // Compute the transmittance and sampling density
    Vec3 Tr = Exp(-sigma_t * std::min(t, MaxReal) * ray.d.Length());

    // Return weighting factor for scattering from homogeneous medium
    Vec3 density = sampledMedium ? (sigma_t * Tr) : Tr;
    real pdf = 0;
    for (int i = 0; i < 3; ++i) pdf += density[i];
    pdf *= 1 / (real)(3);
    //pdf = density[0];
    if (pdf == 0) {
        pdf = 1;
    }
    return sampledMedium ? (Tr * sigma_s / pdf) : (Tr / pdf);
}

Vec3 HomogeneousMedium::EquiAngularSampling(
    const Ray& ray, StateSequence& rand, MemoryArena& arena,
    const Intersection& lightPoint, MediumIntersection* mi) const {
    //real rrProb = Exp(-sigma_t * std::min(ray.tMax * ray.d.Length(), MaxReal)).x;
    //real rrProb = Tr(ray, rand).x;
    //if (rand() > rrProb) return 1.f / (1 - rrProb);
    real tMax = 10;
    //real tMax = std::min((real)10, ray.tMax);


    Vec3 lightPos = lightPoint.hit;
    real delta = Dot(lightPos - ray.o, ray.d);
    real D = (ray.o + delta * ray.d - lightPos).Length();
    real theta_a = std::atan2(0.f - delta, D);
    real theta_b = std::atan2(tMax - delta, D);
    real u = rand();
    real tau = D * std::tan((1 - u) * theta_a + u * theta_b);
    real t = delta + tau;
    bool sampleMedia = t < ray.tMax;
    Vec3 Tr = sampleMedia ? Exp(-sigma_t * t) : this->Tr(ray, rand);
    real pdf = sampleMedia ?
        D / ((theta_b - theta_a) * (D * D + tau * tau))
        : 1 - (std::atan2(std::min(tMax, ray.tMax) - delta, D) - theta_a) / (theta_b - theta_a);
    if(sampleMedia) *mi = MediumIntersection(ray(t), -ray.d, this, ARENA_ALLOC(arena, HenyeyGreenstein)(g));

    return sampleMedia ? Tr * sigma_s / pdf : this->Tr(ray, rand) / pdf;
}
NAMESPACE_END