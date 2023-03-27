#pragma once
#include "WinApp.h"
#include "Sprite.h"
#include "Rhythm.h"
#include <string>

class UIManager
{
public:
	static const int fontSize = 80;
	static const int maxDegree = 360;
	const float maxSpeed = 30.0f;
	static const int maxCharCount = 256;	// 最大文字数
	static const int kBufferSize = 512;   // 書式付き文字列展開用バッファサイズ

	~UIManager();

	void Initialize();

	void Update(Rhythm* rhythm);

	void Draw(Rhythm* rhythm);

	void InitializeScore(UINT texnumber);

	void ScorePrint(float x, float y, float scale, int size, const char* fmt, ...);

private:

	XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f};
	XMFLOAT4 flashColor = { 10.0f,10.0f,10.0f,1.0f };
	float lectPoint = 0.0f;

	XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0.0f;

	float moveSpeed = 0.1f;

	Sprite* textSprite[3];
	Sprite* numbersSprite = nullptr;

	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	// 書式付き文字列展開用バッファ
	char buffer[kBufferSize];

	int j;
};

