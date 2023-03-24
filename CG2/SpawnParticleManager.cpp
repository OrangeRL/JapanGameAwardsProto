#include "SpawnParticleManager.h"
using namespace std;

void SpawnParticleManager::CreateParticle(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale)
{
	particles.emplace_back(new SpawnParticle);
	particles.back()->Initialize(viewProjection, matProjection, beginPos);
	//particles.back()->SetScale(scale);
}

SpawnParticleManager::SpawnParticleManager()
{
}

SpawnParticleManager::~SpawnParticleManager()
{
	particles.clear();
}

void SpawnParticleManager::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{

	for (unique_ptr<SpawnParticle>& particle : particles)
	{
	
	}
}

void SpawnParticleManager::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale)
{
	particles.remove_if([](std::unique_ptr<SpawnParticle>& particle) {
		return particle->IsDead();
		});

	if (particles.size() <= LIMIT)
	{
		CreateParticle(viewProjection, matProjection, beginPos, scale);
	}

	for (unique_ptr<SpawnParticle>& particle : particles)
	{
		particle->Update(beginPos);
	}
}

void SpawnParticleManager::Draw()
{
	for (unique_ptr<SpawnParticle>& particle : particles)
	{
		particle->Draw();
	}
}
