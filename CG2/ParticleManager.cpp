//#include "ParticleManager.h"
//using namespace std;
//
//void ParticleManager::CreateParticle(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale)
//{
//	particles.emplace_back(new Particle2);
//	particles.back()->Initialize(viewProjection, matProjection, L"Resources/red1x1.png");
//	//particles.back()->SetScale(scale);
//}
//
//ParticleManager::ParticleManager()
//{
//}
//
//ParticleManager::~ParticleManager()
//{
//	particles.clear();
//}
//
//void ParticleManager::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
//{
//
//	for (unique_ptr<Particle2>& particle : particles)
//	{
//
//	}
//}
//
//void ParticleManager::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 beginPos, Vector3 scale)
//{
//	particles.remove_if([](std::unique_ptr<Particle2>& particle) {
//		return particle->IsDead();
//		});
//
//	if (particles.size() <= LIMIT)
//	{
//		CreateParticle(viewProjection, matProjection, beginPos, scale);
//	}
//
//	for (unique_ptr<Particle2>& particle : particles)
//	{
//		particle->Update(beginPos);
//	}
//}
//
//void ParticleManager::Draw()
//{
//	for (unique_ptr<Particle2>& particle : particles)
//	{
//		particle->Draw();
//	}
//}
