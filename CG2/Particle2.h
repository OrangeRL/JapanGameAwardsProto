#pragma once
#include "GameObject3D.h"
#include "Player.h"
#include "Audio.h"

class Particle2 {
public:

	Particle2();

	~Particle2();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);
	
	void Update(Vector3 initPos);

	void Draw();

	void Reset();

	int GetIsDead();
	bool IsDead() const { return isDead[particleValue]; }
private:

	static const int particleValue = 15;

	GameObject3D* gameObject[particleValue];
	Model* model;
	Vector3 velocity[particleValue] = {};
	Vector3 angle[particleValue] = {};

	const float speed = 0.3;

	int isDead[particleValue] = { 0 };

	int timer[particleValue] = { 0 };

	int particleOff[particleValue] = { 0 };

	Player* player = nullptr;
	PlayerBullet* playerBullet = nullptr;
	SoundManager soundManager_;
	//‰¹º“Ç‚Ýž‚Ý
	SoundData soundData1 = soundManager_.SoundLoadWave("Resources/death.wav");
	bool isPlayingBGM = false;

	Vector3 spawnPos[particleValue] = {};
	Vector3 enemyPos[particleValue] = {};

	Vector3 posA;
	Vector3 posB;
	Vector3 posC;
};

