#pragma once
#include "SpawnParticle.h"
#include <list>
#include <memory>

class SpawnParticleManager
{
public:
	SpawnParticleManager();
	~SpawnParticleManager();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection,Vector3 beginPos, Vector3 scale = { 0.5f,0.5f,0.5f });
	void Draw();

	void CreateParticle(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale);

	const std::list<std::unique_ptr<SpawnParticle>>& GetParticle() {
		return particles;
	};

	static const int LIMIT = 10;

private:
	std::list<std::unique_ptr<SpawnParticle>> particles;
};