#include "UIManager.h"

UIManager::~UIManager() {

	delete clearSprite;
	delete numbersSprite;
}

void UIManager::Initialize(UINT texnumber) {

	clearSprite = new Sprite(2, { 0.0f,0.0f }, { 256.0f,64.0f }, { 0.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
	clearSprite->Initialize();
	clearSprite->SetAnchorPoint({ 0.5f, 0.5f });
	
	moveSpeed = maxSpeed;
	position = { 0.0f,-250.0f };
	countDown = 3;

	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}

}

void UIManager::Update(Rhythm* rhythm, Input* input, int isDead) {

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

		if (rhythm->GetSoundState().measureCount < 4) {
			UIPrintf({ window_width / 2 - 200, position.y }, { 1.7f,2.0f }, { 0.0f,1.0f,1.0f,1.0f }, 5, " WAVE");
			UIPrintf({ window_width / 2 + 20, position.y - 40}, { 2.8f,2.8f }, { 0.0f,1.0f,1.0f,1.0f }, 2, " %f",rhythm->GetSoundState().wave);
			UIPrintf({ window_width / 2 - 250,position.y + 100 }, { 2.2f,2.5f }, { 0.0f,1.0f,1.0f,1.0f }, 7, " READY?");
		}
	}
	else if (rhythm->GetSoundState().measureCount > 4 && isDead == false)
	{
		position = { 0.0f,-250.0f };
		moveSpeed = maxSpeed;
	}

	//カウントダウンの処理
	if (rhythm->GetSoundState().wave <= 2) {

	}
	//透明度をリセット
	if (rhythm->GetSoundState().timer == 0) {
		color.w = 1.0f;
	}

	//カウントダウン「3,2,1」の処理
	if (rhythm->GetSoundState().measureCount >= 4 && rhythm->GetSoundState().measureCount < 7) {

		XMFLOAT2 size = { 6.0f,5.0f };
		UIPrintf({ window_width / 2 - 300,window_height / 2 - 150 },size, color, 2, " %d",countDown);
		UIPrintf({ window_width / 2 - 300 - (1.0f - color.w) * 300,window_height / 2 - 150 - (1.0f - color.w) * 150 },
				 { size.x * (2.0f - color.w),size.y * (2.0f - color.w) }, {color.x,color.y,color.z,color.w / 2}, 2, " %d", countDown);
		if (rhythm->GetSoundState().timer == 59) {
			countDown -= 1;
		}

	}
	//カウントダウン後「START!」の処理
	else if (rhythm->GetSoundState().measureCount == 7) {
		XMFLOAT2 size = { 4.0f,3.5f };
		UIPrintf({ window_width / 2 - 500,window_height / 2 - 100 }, { size.x,size.y }, color, 7, " START!");
		UIPrintf({ window_width / 2 - 500 - (1.0f - color.w) * 500,window_height / 2 - 100 - (1.0f - color.w) * 100 },
				{ size.x * (2.0f - color.w),size.y * (2.0f - color.w) }, { color.x,color.y,color.z,color.w / 2 }, 7, " START!");
	}
	//1waveが終わったらリセット
	else if (rhythm->GetSoundState().measureCount >= 79) {

		countDown = 3;
	}	

	//クリア時の処理
	if ((rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1) ||
		(rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2)) {

		if (rotation < maxDegree) {
			rotation += 6.0f;
			clearSprite->SetSize({ 512 + maxDegree - rotation,150 + maxDegree - rotation });
			clearSprite->SetColor({ 1.0f,1.0f,0.0f,rotation / maxDegree });
		}
		else {
			if (flashColor.x > 1.0f) {

				flashColor.z -= 0.075f;
				flashColor.x -= 0.05f;
				flashColor.y -= 0.05f;


			}
			else {
				if (flame < maxFlame) {
					flame++;
				}
				clearPos.y = MathFunc::easeOutQuint(flame / maxFlame) * 250;
				UIPrintf({ window_width / 2 - 400, window_height - 50  - clearPos.y }, { 2.0f,2.0f }, { 0.0f,1.0f,1.0f,1.0f },12, " TOTAL:%05d",score);
				
			}
			clearSprite->SetColor(flashColor);
		}

		clearSprite->SetPosition({ window_width / 2,window_height / 2 - clearPos.y});

		clearSprite->SetRotation(rotation);

	}
	else {
		rotation = 0.0f;
		flashColor = { 5.0f,5.0f,5.0f,1.0f };
		color.w -= 0.02f;
		clearSprite->SetColor(color);
	}

	//ゲームオーバー時の処理
	if (isDead == true) {
		if (moveSpeed > 0) {
			moveSpeed -= 1.0f;
			position.y += moveSpeed;
		}
		UIPrintf({ window_width / 2 - 600, position.y }, { 3.5f,4.0f }, { 0.0f,1.0f,1.0f,1.0f }, 9, " GAMEOVER");
	}
	
	//ポーズ画面の処理
	if (rhythm->GetSoundState().isPause == true) {
		UIPrintf({ window_width / 2 - 100, window_height / 2 - 50}, { 1.0f,1.0f }, { 0.0f,0.5f,1.0f,1.0f }, 6, " PAUSE");
	}

	//成功失敗判定の処理
	if (input->TriggerKey(DIK_SPACE)) {
		if (rhythm->GetSoundState().isFireActive) {
			judgeAlpha = 1.0f;
		}
	}

	if (rhythm->GetSoundState().judge == Judge::Good) {
		UIPrintf({ window_width / 2 - 200, 100 }, { 2.0f,2.0f }, { 1.0f,1.0f,0.0f,judgeAlpha }, 5, " GOOD");
	}
	else if (rhythm->GetSoundState().judge == Judge::Miss) {
		UIPrintf({ window_width / 2 - 200, 100 }, { 2.0f,2.0f }, { 1.0f,0.2f,0.2f,judgeAlpha }, 5, " MISS");
	}
	judgeAlpha -= 0.05f;

	//コンボ数の表示
	if (rhythm->GetSoundState().combo < 10) {
		size = 2;
	}
	else if (rhythm->GetSoundState().combo < 100) {
		size = 3;
	}
	else {
		size = 4;
	}

	if (rhythm->GetSoundState().combo != 0) {
		UIPrintf({ window_width / 2 - 50 * size, 550 }, { 2.0f,2.0f }, { 0.0f,1.0f,1.0f,1.0f }, size, " %d", rhythm->GetSoundState().combo);
		UIPrintf({ window_width / 2 + 50, 600 }, { 1.0f,1.0f }, { 0.0f,1.0f,1.0f,1.0f }, 6, " COMBO");
	}

	//スコアの表示
	UIPrintf({ window_width - 310, 0 }, { 0.75f,1.0f }, { 0.0f,1.0f,1.0f,1.0f }, 12, " SCORE:%05d",score);
}

void UIManager::Draw(Rhythm* rhythm) {

	if (rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1 ||
		rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2) {
		clearSprite->Draw();
	}

	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}

void UIManager::UIPrintf(XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color, int size, const char* fmt, ...) {
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

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ pos.x  + scale.x * (i * (fontSize - 30)), pos.y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * (float)fontSize, (float)fontIndexY * (float)fontSize }, { (float)fontSize, (float)fontSize });
		spriteDatas[spriteIndex]->SetSize({ (fontSize - 30) * scale.x, fontSize * scale.y });
		spriteDatas[spriteIndex]->SetColor(color);

		// 文字を１つ進める
		spriteIndex++;

	}
}