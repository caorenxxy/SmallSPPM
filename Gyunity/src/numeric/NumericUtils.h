#pragma once
#include "common/Core.h"
#include <cstring>
#include <algorithm>
#include <cmath>

GY_NAMESPACE_BEGIN

GY_FORCE_INLINE real_bit FloatToBits(real f) {
	real_bit ui;
	memcpy(&ui, &f, sizeof(real));
	return ui;
}

GY_FORCE_INLINE real BitsToFloat(real_bit ui) {
	real f;
	memcpy(&f, &ui, sizeof(real_bit));
	return f;
}


GY_FORCE_INLINE real NextFloatUp(real v) {
	// Handle infinity and negative zero for _NextFloatUp()_
	if (std::isinf(v) && v > 0.) return v;
	if (v == (real)-0.f) v = (real)0.f;

	// Advance _v_ to next higher float
	real_bit ui = FloatToBits(v);
	if (v >= 0)
		++ui;
	else
		--ui;
	return BitsToFloat(ui);
}


GY_FORCE_INLINE real NextFloatDown(real v) {
	// Handle infinity and positive zero for _NextFloatDown()_
	if (std::isinf(v) && v < 0.) return v;
	if (v == (real)0.f) v = (real)-0.f;
	real_bit ui = FloatToBits(v);
	if (v > 0)
		--ui;
	else
		++ui;
	return BitsToFloat(ui);
}

GY_FORCE_INLINE real gamma(int n) {
	return (n * MachineEps) / (1 - n * MachineEps);
}


GY_NAMESPACE_END