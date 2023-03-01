#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include "Audio.h"
//#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include "Sprite.h"

#include "Player.h"
#include "Map.h"
#include "Goal.h"
#include "Particle.h"
#include "Enemy.h"
#include "Rhythm.h"
#include "DebugText.h"

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

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	DebugText debugText;

	Rhythm* rhythm = nullptr;

	ViewProjection viewProjection_;

	bool isPlayingBGM = false;

	Player* player = nullptr;

	Particle* particle = nullptr;

	Map* map = nullptr;

	Goal* goal = nullptr;
	Enemy* enemy = nullptr;

	GameObject3D* tutorialFloor = nullptr;
	GameObject3D* stageFloor = nullptr;

	//シーン管理
	enum class Scene
	{
		Title,//タイトル
		Stage,//ステージ
	};

	Scene scene_ = Scene::Title;

	int stage = 1;

	int gameoverTimer = 0;

	int offset = 5;

	Sprite* title_ = nullptr;
	Sprite* clear_ = nullptr;
	Sprite* gameOver_ = nullptr;
	Sprite* spaceToContinue_ = nullptr;
	Sprite* spaceToReturnTitle_ = nullptr;

	Sprite* num_[10];

};