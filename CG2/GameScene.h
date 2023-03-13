#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include "Audio.h"
//#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <sstream>
#include "Sprite.h"
#include <memory>
#include <list>

#include "Player.h"
#include "Map.h"
#include "Goal.h"
#include "Particle.h"
#include "Enemy.h"
#include "Rhythm.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include "ReilCamera.h"
#include "Item.h"

class GameScene {

public: // メンバ関数

	//コンストラクタ
	GameScene();
	/// デストラクタ
	~GameScene();

	//初期化
	void Initialize(WinApp* winApp);

	//毎フレーム処理
	void Update();

	//描画処理
	void Draw();

	void Reset();

	void Collision();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void loadEnemyPopData();
	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommand();


	const std::list<std::unique_ptr<Enemy>>& GetEnemies() { return enemys1; }
private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	DebugText debugText;

	Rhythm* rhythm = nullptr;
	SoundManager soundManager_;
	ViewProjection viewProjection_;

	bool isPlayingBGM = false;

	Player* player = nullptr;
	PlayerBullet* playerBullet = nullptr;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys1;
	std::list<std::unique_ptr<Enemy>> enemys2;
	std::list<std::unique_ptr<EnemyBullet>> bullets1;
	std::list<std::unique_ptr<EnemyBullet>> bullets2;

	//天球
	GameObject3D* skydome = nullptr;

	//アイテム
	Item* item = nullptr;

	Vector3 enemyPos = {};
	//シーン管理
	enum class Scene
	{
		Title,//タイトル
		Stage,//ステージ
	};

	Scene scene_ = Scene::Title;

	int stage = 1;

	int gameoverTimer = 0;

	ReilCamera* reilCamera = nullptr;

	Sprite* num_[10];

// 敵コマンド関係
//------------------------------------
	std::stringstream enemyPopCommand;
	bool waitFlag = false;
	float waitTime_;
//------------------------------------
};