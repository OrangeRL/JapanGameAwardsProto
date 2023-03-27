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

	moveSpeed = maxSpeed;
	position = { 0.0f,-250.0f };

	InitializeScore(15);

}

void UIManager::Update(Rhythm* rhythm) {
	//ScorePrint(100, 0, 1, 5, "%05d",rhythm->GetSoundState().measureCount);

	//最初の「READY?」の処理
	if (rhythm->GetSoundState().measureCount < 4) {	

		if (rhythm->GetSoundState().measureCount < 2) {

			if (moveSpeed > 0) {
				moveSpeed -= 1.0f;
				position.y += moveSpeed;

			}
		}
		else if (rhythm->GetSoundState().measureCount == 3) {
			if (moveSpeed <= maxSpeed) {
				moveSpeed += 1.0f;
				position.y -= moveSpeed;

			}
		}
		textSprite[0]->SetSize({ 400,80 });
		textSprite[1]->SetSize({ 500,100 });
		textSprite[0]->SetTextureRect({ 0,160 }, { 512,fontSize });
		textSprite[1]->SetTextureRect({ 0,240 }, { 512,fontSize });

		textSprite[0]->SetPosition({ window_width / 2 - 200,position.y});
		textSprite[1]->SetPosition({ window_width / 2 - 250,position.y + 100 });
	
		numbersSprite->SetPosition({ window_width / 2 + 50,position.y - 20});
		numbersSprite->SetSize({ 120,120 });
		numbersSprite->SetTextureRect({ rhythm->GetSoundState().wave * fontSize,0 }, { fontSize,fontSize });
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
			numbersSprite->SetTextureRect({240 - lectPoint,0}, { fontSize,fontSize });
		}
		//カウントダウン後「START!」の処理
		else if (rhythm->GetSoundState().measureCount == 7) {

			textSprite[2]->SetPosition({window_width / 2,window_height / 2});
			textSprite[2]->SetSize({800,150});
			textSprite[2]->SetTextureRect({0,0}, {512,fontSize });

		}
		//1waveが終わったらリセット
		else if (rhythm->GetSoundState().measureCount >= 79) {
			numbersSprite->SetPosition({ window_width / 2 + 50,position.y - 20 });
			numbersSprite->SetSize({ 120,120 });
			numbersSprite->SetTextureRect({ rhythm->GetSoundState().wave * fontSize,0 }, { fontSize,fontSize });
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
		if (rotation < maxDegree) {
			rotation += 6.0f;
			textSprite[2]->SetSize({ 800 + maxDegree - rotation,150 + maxDegree - rotation });
			textSprite[2]->SetColor({ 1.0f,1.0f,1.0f,rotation / maxDegree });
		}
		else {
			if (flashColor.x > 1.0f) {

				flashColor.x -= 0.1f;
				flashColor.y -= 0.1f;
				flashColor.z -= 0.1f;

			}
			textSprite[2]->SetColor(flashColor);
		}

		textSprite[2]->SetTextureRect({ 0,80 }, { 512,fontSize });
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

	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}
void UIManager::InitializeScore(UINT texnumber) {
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}


void UIManager::ScorePrint(float x, float y, float scale, int size, const char* fmt, ...) {
	// 全ての文字について
	for (int i = 0; i < size; i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}
		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = buffer[i];

		va_list arg_ptr;

		va_start(arg_ptr, fmt);
		j = vsnprintf(buffer, 64, fmt, arg_ptr);
		va_end(arg_ptr);
		buffer[i] = j;

		int fontIndex = character - 28;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexX = fontIndex % 10;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ x  + scale * (i * (fontSize - 30)), y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontSize, 0 }, { (float)fontSize, (float)fontSize });
		spriteDatas[spriteIndex]->SetSize({ (fontSize - 30) * scale, fontSize * scale });

		// 文字を１つ進める
		spriteIndex++;

	}
}