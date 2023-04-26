#include "Player.h"

class Map {
public:
	int GetObjectCount();
	int GetObjectID(int x, int y);
	WorldTransform GetWorldTransform(int ObjectNum);
};

class Goal {
public:
	WorldTransform GetWorldTransform();
};

class Enemy {
public:
	WorldTransform GetWorldTransform();
	void Reset();
};

class GameScene {
public:
	const std::list<std::unique_ptr<Enemy>>& GetEnemies();
	void Reset();
};

Player::Player() {

}

Player::~Player() {
	delete gameObject;
}

void Player::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	viewProjection_ = viewProjection;

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/playerdemo/playerdemo.obj");
	gameObject->PreLoadTexture(L"Resources/playerdemo/player.png");
	gameObject->SetViewProjection(viewProjection_);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	aimObject = new GameObject3D();
	aimObject->PreLoadModel("Resources/square/square.obj");
	aimObject->PreLoadTexture(L"Resources/red.png");
	aimObject->SetViewProjection(viewProjection_);
	aimObject->SetMatProjection(matProjection);
	aimObject->Initialize();

	aimObject2 = new GameObject3D();
	aimObject2->PreLoadModel("Resources/square/square.obj");
	aimObject2->PreLoadTexture(L"Resources/red.png");
	aimObject2->SetViewProjection(viewProjection_);
	aimObject2->SetMatProjection(matProjection);
	aimObject2->Initialize();

	aimObject3 = new GameObject3D();
	aimObject3->PreLoadModel("Resources/square/square.obj");
	aimObject3->PreLoadTexture(L"Resources/red.png");
	aimObject3->SetViewProjection(viewProjection_);
	aimObject3->SetMatProjection(matProjection);
	aimObject3->Initialize();

	aimObject4 = new GameObject3D();
	aimObject4->PreLoadModel("Resources/square/square.obj");
	aimObject4->PreLoadTexture(L"Resources/red.png");
	aimObject4->SetViewProjection(viewProjection_);
	aimObject4->SetMatProjection(matProjection);
	aimObject4->Initialize();
	
	Reset();

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.initialize();
}

void Player::Update(WorldTransform wt, Vector3 vec) {

	if (life <= 0) {
		isDead = true;
	}

	if (isInvincible == true) {
		invincibleTimer--;

		if (invincibleTimer <= 0.0f) {
			isInvincible = false;
			invincibleTimer = 120;
		}
	}

	playerPos = GetWorldTransform().translation;

	Move();
	//Aim();
	//enemyPos = enemy->GetWorldTransform().translation;
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	for (int i = 0; i < 2; i++) {
		//デスフラグの立った弾を削除
		bulletsSub_[i].remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
			return bullet->IsDead();
			});
	}

	bulletsAim_.remove_if([](std::unique_ptr<Pattern2>& bullet) {
		return bullet->IsDead();
		});

	//弾更新

	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }

	if (isDead == false)
	{
		//aimObject->worldTransform.parent = &wt;
		gameObject->worldTransform.parent = &wt;
		gameObject->Update();
		aimObject->Update();
		aimObject2->Update();
		aimObject3->Update();
		aimObject4->Update();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(vec, 0.0f);
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bulletsSub_[0]) {
		bullet->Update(vec, 0.1f);
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bulletsSub_[1]) {
		bullet->Update(vec, -0.1f);
	}

	for (std::unique_ptr<Pattern2>& bullet : bulletsAim_) {
		bullet->Update(enemyPos, playerPos);
	}
	Collision();

	//SetPos({ 0.0f,0.0f,5.0f });
}

void Player::AimHit() {
	isAimHit = true;
	aimObject->color = { 1.0f,0.0f,0.0f,0.0f };
	aimObject2->color = { 1.0f,0.0f,0.0f,0.0f };
	aimObject3->color = { 1.0f,0.0f,0.0f,0.0f };
	aimObject4->color = { 1.0f,0.0f,0.0f,0.0f };
	aimObject->worldTransform.scale = { 1.5f,1.5f,1.5f };
	aimObject2->worldTransform.scale = { 1.0f,1.0f,1.0f };
	aimObject3->worldTransform.scale = { 0.5f,0.5f,0.5f };
	aimObject4->worldTransform.scale = { 0.5f,0.5f,0.5f };
}
void Player::NotAimHit() {
	isAimHit = false;
	//aimObject->color = { 0.0f,0.0f,0.0f,1.0f };
	//aimObject2->color = { 0.0f,0.0f,0.0f,1.0f };
	//aimObject3->color = { 0.0f,0.0f,0.0f,1.0f };
	//aimObject4->color = { 0.0f,0.0f,0.0f,1.0f };
	aimObject->worldTransform.scale = { 1.5f, 1.5f, 1.5f };
	aimObject2->worldTransform.scale = { 1.0f,1.0f,1.0f };
	aimObject3->worldTransform.scale = { 0.5f,0.5f,0.5f };
	aimObject4->worldTransform.scale = { 0.5f,0.5f,0.5f };
}
void Player::Aim(Vector3 player, Vector3 enemy, Vector3 vec, float shotAngle) {
	
	velocity = { sin(vec.y + shotAngle),-vec.x,cos(vec.y + shotAngle) };
	float speed = -5.0f;
	velocity.x *= speed;
	velocity.y *= speed;
	velocity.z *= speed;

	if (aimObject->worldTransform.translation.z > GetPos().z + 40) {
		aimObject->worldTransform.translation.z = GetPos().z;
	}
	aimObject->worldTransform.translation.x = gameObject->worldTransform.translation.x;
	aimObject->worldTransform.translation.y = gameObject->worldTransform.translation.y;
	aimObject->worldTransform.translation.z = GetPos().z - velocity.z * 1;
	aimObject->worldTransform.rotation.z += 0.01;
	aimObject2->worldTransform.rotation.z -= 0.01;
	aimObject3->worldTransform.rotation.z += 0.01;
	aimObject4->worldTransform.rotation.z -= 0.01;

	aimObject2->worldTransform.translation.x = gameObject->worldTransform.translation.x;
	aimObject2->worldTransform.translation.y = gameObject->worldTransform.translation.y;
	aimObject2->worldTransform.translation.z = GetPos().z - velocity.z * 4;

	aimObject3->worldTransform.translation.x = gameObject->worldTransform.translation.x;
	aimObject3->worldTransform.translation.y = gameObject->worldTransform.translation.y;
	aimObject3->worldTransform.translation.z = GetPos().z - velocity.z * 7;

	aimObject4->worldTransform.translation.x = gameObject->worldTransform.translation.x;
	aimObject4->worldTransform.translation.y = gameObject->worldTransform.translation.y;
	aimObject4->worldTransform.translation.z = GetPos().z - velocity.z * 9;

	aimObject->worldTransform.scale = { 1.5f,1.5f,1.5f };
	aimObject2->worldTransform.scale = { 1.0f,1.0f,1.0f };
	aimObject3->worldTransform.scale = { 0.5f,0.5f,0.5f };
	aimObject4->worldTransform.scale = { 0.5f,0.5f,0.5f };

}
//
//void Player::Aim(Vector3 player, Vector3 enemy) {
//	//自機から3Dレティクルへの距離
//	const float kDistancePlayerTo3DReticle = 50.0f;
//	//自機から3Dレティクルへのオフセット(Z+向き)
//	Vector3 offset = { 0,0,1.0f };
//	//自機のワールド行列の回転を反映
//	//offset = MathFunc::Utility::MulVector3AndMatrix4(offset, gameObject->worldTransform.matWorld);
//	////ベクトルの長さを整える
//	//offset.nomalize();
//	//offset * kDistancePlayerTo3DReticle;
//	//3Dレティクルの座標を設定
//	
//	float speed = -0.2f;
//	posA = player;
//	posB = enemy;
//	posC = posA - posB;
//	//posC.nomalize();
//	//posC *= speed;
//
//	aimObject->worldTransform.scale = { 0.1, 0.1, 50 };
//
//	aimObject->worldTransform.translation = gameObject->worldTransform.translation + offset*kDistancePlayerTo3DReticle;
//}

void Player::Draw() {
	if (isDead == false) {
		if (invincibleTimer % 2 == 0) {
			gameObject->Draw();
		}
		if (isAimHit == true) {
			aimObject->Draw();
		}
		aimObject->Draw();
		aimObject2->Draw();
		aimObject3->Draw();
		aimObject4->Draw();
		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(); }
		for (std::unique_ptr<Pattern2>& bullet : bulletsAim_) { bullet->Draw(); }

		for (int i = 0; i < 2; i++) {
			for (std::unique_ptr<PlayerBullet>& bullet : bulletsSub_[i]) { bullet->Draw(); }
		}
	}
}

void Player::Reset() {
	//gameObject->worldTransform.translation = { -10 , 0 , -10 };
	moveSpeed = 0;
	isDead = false;
}

void Player::Rotate() {

	const float rotationSpeed = MathFunc::Utility::Deg2Rad(6.0f);

	Vector3 rotation = { 0 , 0 , 0 };

	rotation.y = rotationSpeed;

	gameObject->worldTransform.rotation += rotation;

}

void Player::Move() {

	moveSpeed = 0.2f;
	move = { 0,0,0 };

	if (input.PushKey(DIK_W) || input.PushKey(DIK_S) || input.PushKey(DIK_D) || input.PushKey(DIK_A) || input.PushKey(DIK_E) || input.PushKey(DIK_Q))
	{

		// 移動後の座標を計算
		if (input.PushKey(DIK_W) && input.PushKey(DIK_S)) {
			move = { 0,0,0 };
		}
		else if (input.PushKey(DIK_W) && gameObject->worldTransform.translation.y <= moveLimit.y) {
			move = { 0,moveSpeed,0 };
		}
		else if (input.PushKey(DIK_S) && gameObject->worldTransform.translation.y >= -moveLimit.y) {
			move = { 0,-moveSpeed,0 };
		}

		if (input.PushKey(DIK_D) && input.PushKey(DIK_A)) {
			move = { 0,0,0 };
		}
		else if (input.PushKey(DIK_D) && gameObject->worldTransform.translation.x <= moveLimit.x) {
			move = { moveSpeed,0,0 };

			if (gameObject->worldTransform.rotation.z >= -0.5f) {
				gameObject->worldTransform.rotation.z -= 0.01f;
			}
		}

		else if (input.PushKey(DIK_A) && gameObject->worldTransform.translation.x >= -moveLimit.x) {
			move = { -moveSpeed,0,0 };

			if (gameObject->worldTransform.rotation.z <= 0.5f) {
				gameObject->worldTransform.rotation.z += 0.01f;
			}
		}

		if (input.PushKey(DIK_D) && gameObject->worldTransform.translation.x <= moveLimit.x) {
			if (input.PushKey(DIK_W) && gameObject->worldTransform.translation.y <= moveLimit.y) {
				move = { moveSpeed,moveSpeed,0 };
			}
			if (input.PushKey(DIK_S) && gameObject->worldTransform.translation.y >= -moveLimit.y) {
				move = { moveSpeed,-moveSpeed,0 };
			}
		}


		if (input.PushKey(DIK_A) && gameObject->worldTransform.translation.x >= -moveLimit.x) {
			if (input.PushKey(DIK_W) && gameObject->worldTransform.translation.y <= moveLimit.y) {
				move = { -moveSpeed,moveSpeed,0 };
			}
			if (input.PushKey(DIK_S) && gameObject->worldTransform.translation.y >= -moveLimit.y) {
				move = { -moveSpeed,-moveSpeed,0 };
			}
		}

	}

	if (input.PushKey(DIK_D) == 0 || (input.PushKey(DIK_D) && input.PushKey(DIK_A))) {

		if (gameObject->worldTransform.rotation.z < 0) {
			gameObject->worldTransform.rotation.z += 0.01f;
		}
	}
	if (input.PushKey(DIK_A) == 0 || (input.PushKey(DIK_D) && input.PushKey(DIK_A))) {
		if (gameObject->worldTransform.rotation.z > 0) {
			gameObject->worldTransform.rotation.z -= 0.01f;
		}
	}

	gameObject->worldTransform.translation += move;
}

void Player::NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos, Vector3 playerPos, Weapons weapon) {
	playerPos = GetPos();
	//enemyPos = enemy->GetWorldTransform().translation;
	//弾を生成し、初期化
	std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(viewProjection, matProjection, enemyPos, playerPos, weapon);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));

	if (weapon == Weapons::ThreeWay) {
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet1 = std::make_unique<PlayerBullet>();
		newBullet1->Initialize(viewProjection, matProjection, enemyPos, playerPos, weapon);

		//弾を登録する
		bulletsSub_[0].push_back(std::move(newBullet1));

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet2 = std::make_unique<PlayerBullet>();
		newBullet2->Initialize(viewProjection, matProjection, enemyPos, playerPos, weapon);

		//弾を登録する
		bulletsSub_[1].push_back(std::move(newBullet2));
	}

	timer--;
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		//bullet->AttackPress();
		//bullet->Update();
	}
	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = GetBullets();
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		/*if (input.PushKey(DIK_P)) {
			isDead = true;
			bulletA->OnCollision();
		}*/
	}
}

//void Player::NewBulletAim(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos, Vector3 playerPos) {
//	playerPos = GetPos();
//
//	//弾を生成し、初期化
//	std::unique_ptr<Pattern2>newBullet = std::make_unique<Pattern2>();
//	newBullet->Initialize(viewProjection, matProjection);
//
//	//弾を登録する
//	bulletsAim_.push_back(std::move(newBullet));
//
//	for (std::unique_ptr<Pattern2>& bullet : bulletsAim_) {
//		bullet->Update(enemyPos,playerPos);
//	}
//
//}
void Player::Collision() {

	//const std::list < std::unique_ptr<Enemy>>& enemyLoads = GetEnemyBullets();
	//for (const std::unique_ptr<Enemy>& enemy : enemyLoads) {

	////enemy-player
	//if (enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x < 2 &&
	//	-2 < enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x) {
	//	if (enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y < 3 &&
	//		-3 < enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y) {
	//		if (enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z) {
	//			if (life > 0) {
	//				life -= 10;
	//			}
	//			else {
	//				isDead = true;
	//			}
	//		}
	//	}
	//}
	////bullet-enemy
	//
	//
	//

	//const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = GetBullets();
	//for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
	//		if (input.PushKey(DIK_P)) {
	//			isDead = true;
	//			bulletA->OnCollision();
	//		}
	//	if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
	//		-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//		if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//			if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 3 &&
	//				-3 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//				bulletA->OnCollision();
	//				enemy->Reset();
	//				isDead=true;
	//			}
	//		}
	//	}
	//}
	//}

}

//アクセッサ
void Player::SetMap(Map* map) {
	this->map = map;
}

void Player::SetGoal(Goal* goal) {
	this->goal = goal;
}

int Player::GetIsGoal() {
	return isGoal;
}

void Player::SetIsGoal(int flag) {
	isGoal = flag;
}

void Player::SetEnemy(Enemy* enemy) {
	enemy = enemy;
}

int Player::GetIsEnemy() {
	return isGoal;
}

void Player::SetIsEnemy(int flag) {
	isEnemy = flag;
}

int Player::GetIsDead() {
	return isDead;
}

int Player::GetIsAimHit() {
	return isAimHit;
}

Vector3 Player::GetAngle() {
	return angle;
}

WorldTransform Player::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 Player::GetPos() {
	return {
		gameObject->worldTransform.matWorld.m[3][0],
		gameObject->worldTransform.matWorld.m[3][1],
		gameObject->worldTransform.matWorld.m[3][2],
	};
}

Vector3 Player::GetAimPos() {
	return {
		aimObject->worldTransform.matWorld.m[3][0],
		aimObject->worldTransform.matWorld.m[3][1],
		aimObject->worldTransform.matWorld.m[3][2],
	};
}

void Player::OnCollision(Rhythm* rhythm) {

	if (isInvincible == false) {
		life--;
		rhythm->DamageSoundPlay();
	}

	isInvincible = true;
}