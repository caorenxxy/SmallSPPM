#pragma once

#include "common/Core.h"
#include "math/Linagl.h"
#include "math/AABB.h"
#include "system/Threading.h"
#include <mutex>

GYT_NAMESPACE_BEGIN

template<typename T>
class HashGrid 
{
public:
	HashGrid(real irad = 0.8) : mRearchRadius(irad) {}

	void Initialize(real irad) {
		mRearchRadius = irad;
		mHitPointsBBox.Reset();
	}


	// spatial hash function
	inline uint32 hash(const int ix, const int iy, const int iz) {
		return (uint32)((ix * 73856093) ^ (iy * 19349663) ^ (iz * 83492791)) % mHashNum;
	}

	inline void AddPoint(const std::pair<Vec3, T> &point) {
		mHitPoints.push_back(point);
		mHitPointsBBox.Fit(point.first);
	}

	inline void AddPoint(std::pair<Vec3, T> &&point) {
		mHitPoints.push_back(point);
		mHitPointsBBox.Fit(point.first);
	}

	inline void AddPoint(const std::pair<Vec3, T> &point, real radius) {
		mHitPoints.push_back(point);
		mRad.push_back(radius);
		mHitPointsBBox.Fit(point.first);
	}

	inline void AddPoint(std::pair<Vec3, T> &&point, real radius) {
		mHitPoints.push_back(point);
		mRad.push_back(radius);
		mHitPointsBBox.Fit(point.first);
	}

	void BuildHashGrid(real maxRadius = 0.8f) {
		// find the bounding box of all the measurement points
		//hpbbox.Reset();
		//for (const std::pair<Vec3, T> &hp : hitPoints) {
		//	hpbbox.Fit(hp.first);
		//}

		real searchRadius = maxRadius;
		// heuristic for initial radius
		//Vec3 ssize = hpbbox.maxPoint - hpbbox.minPoint;
		// determine hash table size
		// we now find the bounding box of all the measurement points inflated by the initial radius
		mHitPointsBBox.Reset();
		int64 vphoton = 0;

		for (const std::pair<Vec3, T> &hp : mHitPoints) {
			vphoton++;
			mHitPointsBBox.Fit(hp.first - searchRadius);
			mHitPointsBBox.Fit(hp.first + searchRadius);
		}

		// make each grid cell two times larger than the initial radius
		mInvHashCellSize = 1.f / (searchRadius * 2.f);
		mHashNum = vphoton;

		// build the hash table

		mHashGrid.resize(mHashNum);
		mHashGridLocks.resize(mHashNum);

		int hitPointNum = (int)mHitPoints.size();
		ParallelFor(0, hitPointNum, [&](int i) {
			std::pair<Vec3, T> &hp = mHitPoints[i];
				Vec3 BMin = ((hp.first - mRad[i]) - mHitPointsBBox.minPoint) * mInvHashCellSize;
				Vec3 BMax = ((hp.first + mRad[i]) - mHitPointsBBox.minPoint) * mInvHashCellSize;
				for (int iz = std::abs(int(BMin.z)); iz <= std::abs(int(BMax.z)); iz++)
				{
					for (int iy = std::abs(int(BMin.y)); iy <= std::abs(int(BMax.y)); iy++)
					{
						for (int ix = std::abs(int(BMin.x)); ix <= std::abs(int(BMax.x)); ix++)
						{
							int hv = hash(ix, iy, iz);
							std::lock_guard<Spinlock> lock(mHashGridLocks[hv]);
							mHashGrid[hv].push_back(hp.second);
						}
					}
				}
		});
	}

	std::vector<T>& GetGrid(const Vec3 &point) {
		Vec3 hh = (point - mHitPointsBBox.minPoint) * mInvHashCellSize;
		int ix = std::abs(int(hh.x)), iy = std::abs(int(hh.y)), iz = std::abs(int(hh.z));
		return mHashGrid[hash(ix, iy, iz)];
	}


	void ClearHashGrid() {
		//std::vector<std::vector<T>>().swap(hashGrid);
		//std::vector<std::pair<Vec3, T>>().swap(hitPoints);
		mHashGrid.clear();
		mHitPoints.clear();
		mRad.clear();
		mHitPointsBBox.Reset();
	}

private:
	std::vector<std::vector<T>> mHashGrid;
	std::vector<std::pair<Vec3, T>> mHitPoints;
	std::vector<real> mRad;
	std::vector<Spinlock> mHashGridLocks;
	int64 mHashNum;
	real mInvHashCellSize;
	real mRearchRadius;
	AABB mHitPointsBBox;
};


GYT_NAMESPACE_END