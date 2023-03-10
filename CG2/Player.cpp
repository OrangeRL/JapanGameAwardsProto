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

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(L"Resources/star/star.jpg");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Reset();
}

void Player::Update() {
	playerPos = GetWorldTransform().translation;
	Move();
	//enemyPos = enemy->GetWorldTransform().translation;
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//弾更新
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(enemyPos, GetWorldTransform().translation); }
	
	if (isDead == false)
	{
		gameObject->Update();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }
	//Collision();
}

void Player::Draw() {
	if (isDead == false) {
		gameObject->Draw();
		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(); }
	}
}

void Player::Reset() {
	gameObject->worldTransform.translation = { -10 , 0 , -10 };
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

	moveSpeed = 0.5f;
	move = { 0,0,0 };

	if (input.PushKey(DIK_W) || input.PushKey(DIK_S) || input.PushKey(DIK_D) || input.PushKey(DIK_A) || input.PushKey(DIK_E) || input.PushKey(DIK_Q))
	{
		// 移動後の座標を計算
		if (input.PushKey(DIK_W)) { move = { 0,moveSpeed,0 }; }
		else if (input.PushKey(DIK_S)) { move = { 0,-moveSpeed,0 }; }
		if (input.PushKey(DIK_D)) { move = { moveSpeed,0,0 }; }
		else if (input.PushKey(DIK_A)) { move = { -moveSpeed,0,0 }; }

		if (input.PushKey(DIK_E)) { move = { 0,0,0.1 }; }
		else if (input.PushKey(DIK_Q)) { move = { 0,0,-0.1 }; }

		if (input.PushKey(DIK_D) && input.PushKey(DIK_W)) { move = { moveSpeed,moveSpeed,0 }; }
		else if (input.PushKey(DIK_D) && input.PushKey(DIK_S)) { move = { moveSpeed,-moveSpeed,0 }; }

		if (input.PushKey(DIK_A) && input.PushKey(DIK_W)) { move = { -moveSpeed,moveSpeed,0 }; }
		if (input.PushKey(DIK_A) && input.PushKey(DIK_S)) { move = { -moveSpeed,-moveSpeed,0 }; }
	}


	gameObject->worldTransform.translation += move;
}
void Player::NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos, Vector3 playerPos) {
		playerPos = GetWorldTransform().translation;
		//enemyPos = enemy->GetWorldTransform().translation;
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(viewProjection, matProjection, enemyPos, playerPos);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	
}
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

Vector3 Player::GetAngle() {
	return angle;
}

WorldTransform Player::GetWorldTransform() {
	return gameObject->worldTransform;
}

void Player::OnCollision() {
	isDead = true;
}