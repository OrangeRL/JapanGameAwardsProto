#pragma once
#include "Particle2.h"
#include <list>
#include <memory>

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale = { 0.5f,0.5f,0.5f });
	void Draw();

	void CreateParticle(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale);

	const std::list<std::unique_ptr<Particle2>>& GetParticle() {
		return particles;
	};

	static const int LIMIT = 1;

private:
	std::list<std::unique_ptr<Particle2>> particles;
};


