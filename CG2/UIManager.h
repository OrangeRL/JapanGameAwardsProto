#pragma once
#include "WinApp.h"
#include "Sprite.h"
#include "Rhythm.h"
#include "MathFunc.h"
#include <string>

class UIManager
{
public:
	static const int fontSize = 64;
	static const int maxDegree = 360;
	const float maxSpeed = 30.0f;
	static const int maxCharCount = 256;	// 最大文字数
	static const int kBufferSize = 512;   // 書式付き文字列展開用バッファサイズ
	static const int fontLineCount = 10;

	~UIManager();

	void Initialize(UINT texnumber);

	void Update(Rhythm* rhythm,Input* input, int isDead);

	void Draw(Rhythm* rhythm);

	void UIPrintf(XMFLOAT2 pos,XMFLOAT2 scale, XMFLOAT4 color, int size, const char* fmt, ...);

private:
	const float maxFlame = 50.0f;

	int score = 5000;
	int countDown = 3;

	XMFLOAT4 color = {1.0f,1.0f,0.0f,1.0f};
	XMFLOAT4 flashColor = { 10.0f,10.0f,10.0f,1.0f };
	float lectPoint = 0.0f;

	XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0.0f;
	float moveSpeed = 0.1f;

	XMFLOAT2 clearPos = { 0.0f,0.0f };
	float flame = 0.0f;
	//判定表示の透明度
	float judgeAlpha = 0.0f;
	//表示する文字数
	float size = 2;

	Sprite* clearSprite;
	Sprite* numbersSprite = nullptr;

	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	// 書式付き文字列展開用バッファ
	char buffer[kBufferSize];

	int j;
};

