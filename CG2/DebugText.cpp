#include "DebugText.h"
#include <stdarg.h>
#include <stdio.h>

DebugText::DebugText()
{
}

DebugText::~DebugText()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

void DebugText::Initialize(UINT texnumber)
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void DebugText::Print(const std::string & text, float x, float y, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// 文字を１つ進める
		spriteIndex++;
	}
}

// 書式付き文字列追加
void DebugText::Printf(float x, float y, float scale, int size, const char* fmt, ...) {

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
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// 文字を１つ進める
		spriteIndex++;

	}
}

// まとめて描画
void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}