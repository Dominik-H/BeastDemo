#include "SpriteEffect.h"


SpriteEffect::SpriteEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	SpriteTech = fx->GetTechniqueByName("SpriteTech");
	SpriteMap = fx->GetVariableByName("gSpriteTex")->AsShaderResource();
}

SpriteEffect::~SpriteEffect()
{
}
