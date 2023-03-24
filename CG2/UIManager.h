#pragma once
#include "WinApp.h"
#include "Sprite.h"
#include "Rhythm.h"

class UIManager
{
public:
	~UIManager();

	void Initialize();

	void Update(Rhythm* rhythm);

	void Draw(Rhythm* rhythm);

private:
	const float TEXT_SIZE = 80.0f;
	const float MAX_DEGREE = 360.0f;

	XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f};
	XMFLOAT4 flashColor = { 10.0f,10.0f,10.0f,1.0f };
	float lectPoint = 0.0f;

	XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0.0f;

	const float MAX_SPEED = 30.0f;
	float moveSpeed = 0.1f;

	Sprite* textSprite[3];
	Sprite* numbersSprite = nullptr;
};

