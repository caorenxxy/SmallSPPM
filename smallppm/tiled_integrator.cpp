#include "tiled_integrator.h"
#include "sampler.h"
#include "sampler_enum.h"

NAMESPACE_BEGIN

void TiledIntegrator::Render(const Scene& scene, const Camera& camera) {

	fprintf(stderr, "Rendering ...\n");

	std::shared_ptr<Film> film = camera.GetFilm();
	int resX = film->resX, resY = film->resY;
	const int tileSize = 32;
	std::vector<Tile> tiles;
	int tileIndex = 0;
	for (int y = 0; y < resY; y += tileSize) {
		for (int x = 0; x < resX; x += tileSize) {
			Tile tile;
			tile.minX = x;
			tile.minY = y;
			tile.maxX = std::min(resX, x + tileSize);
			tile.maxY = std::min(resY, y + tileSize);
			tiles.push_back(tile);
		}
	}
	
	std::atomic<int64> workDone = 0;
	const int tileNum = tiles.size();
	ParallelFor(0, tileNum, [&](int i) {
		MemoryArena arena;
		const Tile& tile = tiles[i];
		for (int y = tile.minY; y < tile.maxY; ++y) {
			for (int x = tile.minX; x < tile.maxX; ++x) {
				//DEBUG_PIXEL(460, 654, ThreadIndex());
				for (int s = 0; s < spp; ++s) {
					uint64 instance = samplerEnum->GetIndex(s, x, y);
					RandomStateSequence rand(sampler, instance);
					real u = samplerEnum->SampleX(x, rand());
					real v = samplerEnum->SampleY(y, rand());
					Vec2 pixelSample(u, v);
					Ray ray = camera.GenerateRay(x, y, pixelSample);

					Vec3 L = Li(ray, scene, rand, arena);
					DEBUG_PIXEL_IF(ThreadIndex()) {
						std::cout << "L: " <<  L << std::endl;
					}
					film->AddSample(x + u, y + v, L);
					arena.Reset();
				}
			}
		}
		workDone += 1;
		real percentage = 100.f * workDone / tileNum;
		fprintf(stderr, "\rPercentage: %5.2f%%", percentage);
	});
}

NAMESPACE_END