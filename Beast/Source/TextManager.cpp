#include "TextManager.h"

std::vector<std::wstring> TextManager::texts; 
FontSheet TextManager::mFont;
SpriteBatch TextManager::mSpriteBatch;

void TextManager::Init(ID3D11Device *device)
{
	HR(mFont.Initialize(device, L"Arial", 15.0f, FontSheet::FontStyleRegular, true));
	HR(mSpriteBatch.Initialize(device));
}

void TextManager::DrawAll(ID3D11DeviceContext* dc)
{
	for (UINT idx = 0; idx < texts.size(); ++idx)
	{
		/*
		// Calculate the text width.
		int textWidth = 0;
		for (UINT i = 0; i < texts[idx].size(); ++i)
		{
			WCHAR character = texts[idx][i];
			if (character == ' ')
			{
				textWidth += mFont.GetSpaceWidth();
			}
			else
			{
				const CD3D11_RECT& r = mFont.GetCharRect(texts[idx][i]);
				textWidth += (r.right - r.left + 1);
			}
		}
		*/
		
		// Center the text in the screen.
		POINT textPos;
		textPos.x = 2; //(mClientWidth - textWidth) / 2;
		textPos.y = 0 + (idx * (mFont.GetCharHeight() + 2));//(mClientHeight - mFont.GetCharHeight()) / 2;

		mSpriteBatch.DrawString(dc, mFont, texts[idx], textPos, XMCOLOR(0xff0088ff));
	}
}
