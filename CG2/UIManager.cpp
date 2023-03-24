#include "UIManager.h"

UIManager::~UIManager() {
	for (int i = 0; i < 3; i++) {
		delete textSprite[i];

	}
	
	delete numbersSprite;
}

void UIManager::Initialize() {

	for (int i = 0; i < 3; i++) {
		textSprite[i]->LoadTexture(12 + i, L"Resources/text.png");
		textSprite[i] = new Sprite(12 + i, { 0.0f,0.0f }, { 1000,500 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
		textSprite[i]->Initialize();
	}
	textSprite[2]->SetAnchorPoint({ 0.4f, 0.5f });

	textSprite[0]->SetPosition({ window_width / 2 - 300, -100 });
	textSprite[1]->SetPosition({ window_width / 2 - 300, -100 });

	numbersSprite->LoadTexture(15, L"Resources/numbers.png");
	numbersSprite = new Sprite(15, { window_width / 2 - 200,-100 }, { 1000,500 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
	numbersSprite->Initialize();

	moveSpeed = MAX_SPEED;
	position = { 0.0f,-250.0f };

}

void UIManager::Update(Rhythm* rhythm) {

	//最初の「READY?」の処理
	if (rhythm->GetSoundState().measureCount < 4) {	

		if (rhythm->GetSoundState().measureCount < 2) {

			if (moveSpeed > 0) {
				moveSpeed -= 1.0f;
				position.y += moveSpeed;

			}
		}
		else if (rhythm->GetSoundState().measureCount == 3) {
			if (moveSpeed <= MAX_SPEED) {
				moveSpeed += 1.0f;
				position.y -= moveSpeed;

			}
		}
		textSprite[0]->SetSize({ 400,80 });
		textSprite[1]->SetSize({ 500,100 });
		textSprite[0]->SetTextureRect({ 0,160 }, { 512,TEXT_SIZE });
		textSprite[1]->SetTextureRect({ 0,240 }, { 512,TEXT_SIZE });

		textSprite[0]->SetPosition({ window_width / 2 - 200,position.y});
		textSprite[1]->SetPosition({ window_width / 2 - 250,position.y + 100 });
	
		numbersSprite->SetPosition({ window_width / 2 + 50,position.y - 20});
		numbersSprite->SetSize({ 120,120 });
		numbersSprite->SetTextureRect({ rhythm->GetSoundState().wave * TEXT_SIZE,0 }, { TEXT_SIZE,TEXT_SIZE });
	}

	//カウントダウンの処理
	if (rhythm->GetSoundState().wave <= 2) {

		//透明度をリセット
		if (rhythm->GetSoundState().timer == 0) {
			color.w = 1.0f;
		}

		//カウントダウン「3,2,1」の処理
		if (rhythm->GetSoundState().measureCount >= 4 && rhythm->GetSoundState().measureCount < 7) {

			if (rhythm->GetSoundState().timer == 59) {
				lectPoint += 80.0f;
			}
			numbersSprite->SetPosition({window_width / 2 - 100,window_height / 2 - 100});
			numbersSprite->SetSize({200,200});
			numbersSprite->SetTextureRect({240 - lectPoint,0}, { TEXT_SIZE,TEXT_SIZE });
		}
		//カウントダウン後「START!」の処理
		else if (rhythm->GetSoundState().measureCount == 7) {

			textSprite[2]->SetPosition({window_width / 2,window_height / 2});
			textSprite[2]->SetSize({800,150});
			textSprite[2]->SetTextureRect({0,0}, {512,TEXT_SIZE });

		}
		//1waveが終わったらリセット
		else if (rhythm->GetSoundState().measureCount >= 79) {
			numbersSprite->SetPosition({ window_width / 2 + 50,position.y - 20 });
			numbersSprite->SetSize({ 120,120 });
			numbersSprite->SetTextureRect({ rhythm->GetSoundState().wave * TEXT_SIZE,0 }, { TEXT_SIZE,TEXT_SIZE });
			lectPoint = 0;
		}

	
	}


	if (rhythm->GetSoundState().measureCount >= 4) {
		numbersSprite->SetColor(color);
	}
	else {
		numbersSprite->SetColor({1.0f,1.0f,1.0f,1.0f});
	}

	//クリア時の処理
	if ((rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1) ||
		(rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2)) {
		if (rotation < MAX_DEGREE) {
			rotation += 6.0f;
			textSprite[2]->SetSize({ 800 + MAX_DEGREE - rotation,150 + MAX_DEGREE - rotation });
			textSprite[2]->SetColor({ 1.0f,1.0f,1.0f,rotation / MAX_DEGREE });
		}
		else {
			if (flashColor.x > 1.0f) {

				flashColor.x -= 0.1f;
				flashColor.y -= 0.1f;
				flashColor.z -= 0.1f;

			}
			textSprite[2]->SetColor(flashColor);
		}

		textSprite[2]->SetTextureRect({ 0,80 }, { 512,TEXT_SIZE });
		textSprite[2]->SetPosition({ window_width / 2,window_height / 2 });

		textSprite[2]->SetRotation(rotation);

	}
	else {
		rotation = 0.0f;
		flashColor = { 10.0f,10.0f,10.0f,1.0f };
		color.w -= 0.02f;
		textSprite[2]->SetColor(color);
	}
}

void UIManager::Draw(Rhythm* rhythm) {

	if (rhythm->GetSoundState().measureCount < 4) {
		textSprite[0]->Draw();
		textSprite[1]->Draw();
	}

	if (rhythm->GetSoundState().measureCount < 7) {
		numbersSprite->Draw();
	}
	else if (rhythm->GetSoundState().measureCount >= 7 && rhythm->GetSoundState().measureCount < 8 ||
		rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1 ||
		rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2) {
		textSprite[2]->Draw();
	}
}