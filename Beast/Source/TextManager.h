#pragma once

#include "SpriteBatch.h"
#include "FontSheet.h"

class TextManager
{
public:
	static void Init(ID3D11Device *device);
	static void DrawAll(ID3D11DeviceContext* dc);

	static std::vector<std::wstring> texts;
private:
	static FontSheet mFont;
	static SpriteBatch mSpriteBatch;
};

