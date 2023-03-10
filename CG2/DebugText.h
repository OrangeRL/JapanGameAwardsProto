#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

/// <summary>
/// デバッグ用文字表示
/// </summary>
class DebugText
{
public:	
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256;	// 最大文字数
	static const int fontWidth = 9;			// フォント画像内1文字分の横幅
	static const int fontHeight = 18;		// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;	// フォント画像内1行分の文字数
	static const int kBufferSize = 512;   // 書式付き文字列展開用バッファサイズ

	DebugText();
	~DebugText();

	void Initialize(UINT texnumber);

	// 文字列追加
	void Print(const std::string & text, float x, float y, float scale);

	// 書式付き文字列追加
	void Printf(float x, float y, float scale,int size, const char* fmt, ...);

	void DrawAll(ID3D12GraphicsCommandList * cmdList);

private:

	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	// 書式付き文字列展開用バッファ
	char buffer[kBufferSize];

	int j;
};
