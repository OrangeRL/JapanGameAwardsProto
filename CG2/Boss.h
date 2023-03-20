#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"

enum BossPhase {
	spown,
	attack,
	defence,
};

class Boss
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update();
	void Draw();
private:
	GameObject3D* gameObject = nullptr;
};

