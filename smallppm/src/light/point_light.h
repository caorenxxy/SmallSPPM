#pragma once
#include "light.h"

NAMESPACE_BEGIN

class PointLight : public Light {
public:
	PointLight(const Vec3& p, const Vec3& intensity) : pLight(p), I(intensity) {}

	Vec3 Emission() const override { return I; }

	bool IsDeltaLight() const override { return true; }

	Vec3 Sample_Li(const Intersection& isect, Vec3* wi, real* pdf, Intersection* lightPoint, const Vec2& u) const override;

	real Pdf_Li(const Intersection& isect, const Vec3& wi) const override {
		return 0;
	}

	Vec3 Power() const override {
		return 4 * PI * I;
	}

	Vec3 SampleLight(Intersection* isect, Vec3* dir, real* pdfPos, real* pdfDir, const Vec2& u, const Vec2& v) const override;

	Vec3 SampleOnePoint(Intersection* isect, real* pdf, const Vec2& u) const override {
		isect->hit = pLight;
		*pdf = 1.f;
		return I;
	}
private:
	Vec3 pLight;
	Vec3 I;
};

NAMESPACE_END