#include "NormalMapEffect.h"

NormalMapEffect::NormalMapEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	Light1Tech    = fx->GetTechniqueByName("Light1");
	Light2Tech    = fx->GetTechniqueByName("Light2");
	Light3Tech    = fx->GetTechniqueByName("Light3");

	Light0TexTech = fx->GetTechniqueByName("Light0Tex");
	Light1TexTech = fx->GetTechniqueByName("Light1Tex");
	Light2TexTech = fx->GetTechniqueByName("Light2Tex");
	Light3TexTech = fx->GetTechniqueByName("Light3Tex");

	Light0TexAlphaClipTech = fx->GetTechniqueByName("Light0TexAlphaClip");
	Light1TexAlphaClipTech = fx->GetTechniqueByName("Light1TexAlphaClip");
	Light2TexAlphaClipTech = fx->GetTechniqueByName("Light2TexAlphaClip");
	Light3TexAlphaClipTech = fx->GetTechniqueByName("Light3TexAlphaClip");

	Light1FogTech    = fx->GetTechniqueByName("Light1Fog");
	Light2FogTech    = fx->GetTechniqueByName("Light2Fog");
	Light3FogTech    = fx->GetTechniqueByName("Light3Fog");

	Light0TexFogTech = fx->GetTechniqueByName("Light0TexFog");
	Light1TexFogTech = fx->GetTechniqueByName("Light1TexFog");
	Light2TexFogTech = fx->GetTechniqueByName("Light2TexFog");
	Light3TexFogTech = fx->GetTechniqueByName("Light3TexFog");

	Light0TexAlphaClipFogTech = fx->GetTechniqueByName("Light0TexAlphaClipFog");
	Light1TexAlphaClipFogTech = fx->GetTechniqueByName("Light1TexAlphaClipFog");
	Light2TexAlphaClipFogTech = fx->GetTechniqueByName("Light2TexAlphaClipFog");
	Light3TexAlphaClipFogTech = fx->GetTechniqueByName("Light3TexAlphaClipFog");

	Light1ReflectTech    = fx->GetTechniqueByName("Light1Reflect");
	Light2ReflectTech    = fx->GetTechniqueByName("Light2Reflect");
	Light3ReflectTech    = fx->GetTechniqueByName("Light3Reflect");

	Light0TexReflectTech = fx->GetTechniqueByName("Light0TexReflect");
	Light1TexReflectTech = fx->GetTechniqueByName("Light1TexReflect");
	Light2TexReflectTech = fx->GetTechniqueByName("Light2TexReflect");
	Light3TexReflectTech = fx->GetTechniqueByName("Light3TexReflect");

	Light0TexAlphaClipReflectTech = fx->GetTechniqueByName("Light0TexAlphaClipReflect");
	Light1TexAlphaClipReflectTech = fx->GetTechniqueByName("Light1TexAlphaClipReflect");
	Light2TexAlphaClipReflectTech = fx->GetTechniqueByName("Light2TexAlphaClipReflect");
	Light3TexAlphaClipReflectTech = fx->GetTechniqueByName("Light3TexAlphaClipReflect");

	Light1FogReflectTech    = fx->GetTechniqueByName("Light1FogReflect");
	Light2FogReflectTech    = fx->GetTechniqueByName("Light2FogReflect");
	Light3FogReflectTech    = fx->GetTechniqueByName("Light3FogReflect");

	Light0TexFogReflectTech = fx->GetTechniqueByName("Light0TexFogReflect");
	Light1TexFogReflectTech = fx->GetTechniqueByName("Light1TexFogReflect");
	Light2TexFogReflectTech = fx->GetTechniqueByName("Light2TexFogReflect");
	Light3TexFogReflectTech = fx->GetTechniqueByName("Light3TexFogReflect");

	Light0TexAlphaClipFogReflectTech = fx->GetTechniqueByName("Light0TexAlphaClipFogReflect");
	Light1TexAlphaClipFogReflectTech = fx->GetTechniqueByName("Light1TexAlphaClipFogReflect");
	Light2TexAlphaClipFogReflectTech = fx->GetTechniqueByName("Light2TexAlphaClipFogReflect");
	Light3TexAlphaClipFogReflectTech = fx->GetTechniqueByName("Light3TexAlphaClipFogReflect");

	Light1SkinnedTech    = fx->GetTechniqueByName("Light1Skinned");
	Light2SkinnedTech    = fx->GetTechniqueByName("Light2Skinned");
	Light3SkinnedTech    = fx->GetTechniqueByName("Light3Skinned");

	Light0TexSkinnedTech = fx->GetTechniqueByName("Light0TexSkinned");
	Light1TexSkinnedTech = fx->GetTechniqueByName("Light1TexSkinned");
	Light2TexSkinnedTech = fx->GetTechniqueByName("Light2TexSkinned");
	Light3TexSkinnedTech = fx->GetTechniqueByName("Light3TexSkinned");

	Light0TexAlphaClipSkinnedTech = fx->GetTechniqueByName("Light0TexAlphaClipSkinned");
	Light1TexAlphaClipSkinnedTech = fx->GetTechniqueByName("Light1TexAlphaClipSkinned");
	Light2TexAlphaClipSkinnedTech = fx->GetTechniqueByName("Light2TexAlphaClipSkinned");
	Light3TexAlphaClipSkinnedTech = fx->GetTechniqueByName("Light3TexAlphaClipSkinned");

	Light1FogSkinnedTech    = fx->GetTechniqueByName("Light1FogSkinned");
	Light2FogSkinnedTech    = fx->GetTechniqueByName("Light2FogSkinned");
	Light3FogSkinnedTech    = fx->GetTechniqueByName("Light3FogSkinned");

	Light0TexFogSkinnedTech = fx->GetTechniqueByName("Light0TexFogSkinned");
	Light1TexFogSkinnedTech = fx->GetTechniqueByName("Light1TexFogSkinned");
	Light2TexFogSkinnedTech = fx->GetTechniqueByName("Light2TexFogSkinned");
	Light3TexFogSkinnedTech = fx->GetTechniqueByName("Light3TexFogSkinned");

	Light0TexAlphaClipFogSkinnedTech = fx->GetTechniqueByName("Light0TexAlphaClipFogSkinned");
	Light1TexAlphaClipFogSkinnedTech = fx->GetTechniqueByName("Light1TexAlphaClipFogSkinned");
	Light2TexAlphaClipFogSkinnedTech = fx->GetTechniqueByName("Light2TexAlphaClipFogSkinned");
	Light3TexAlphaClipFogSkinnedTech = fx->GetTechniqueByName("Light3TexAlphaClipFogSkinned");

	Light1ReflectSkinnedTech    = fx->GetTechniqueByName("Light1ReflectSkinned");
	Light2ReflectSkinnedTech    = fx->GetTechniqueByName("Light2ReflectSkinned");
	Light3ReflectSkinnedTech    = fx->GetTechniqueByName("Light3ReflectSkinned");

	Light0TexReflectSkinnedTech = fx->GetTechniqueByName("Light0TexReflectSkinned");
	Light1TexReflectSkinnedTech = fx->GetTechniqueByName("Light1TexReflectSkinned");
	Light2TexReflectSkinnedTech = fx->GetTechniqueByName("Light2TexReflectSkinned");
	Light3TexReflectSkinnedTech = fx->GetTechniqueByName("Light3TexReflectSkinned");

	Light0TexAlphaClipReflectSkinnedTech = fx->GetTechniqueByName("Light0TexAlphaClipReflectSkinned");
	Light1TexAlphaClipReflectSkinnedTech = fx->GetTechniqueByName("Light1TexAlphaClipReflectSkinned");
	Light2TexAlphaClipReflectSkinnedTech = fx->GetTechniqueByName("Light2TexAlphaClipReflectSkinned");
	Light3TexAlphaClipReflectSkinnedTech = fx->GetTechniqueByName("Light3TexAlphaClipReflectSkinned");

	Light1FogReflectSkinnedTech    = fx->GetTechniqueByName("Light1FogReflectSkinned");
	Light2FogReflectSkinnedTech    = fx->GetTechniqueByName("Light2FogReflectSkinned");
	Light3FogReflectSkinnedTech    = fx->GetTechniqueByName("Light3FogReflectSkinned");

	Light0TexFogReflectSkinnedTech = fx->GetTechniqueByName("Light0TexFogReflectSkinned");
	Light1TexFogReflectSkinnedTech = fx->GetTechniqueByName("Light1TexFogReflectSkinned");
	Light2TexFogReflectSkinnedTech = fx->GetTechniqueByName("Light2TexFogReflectSkinned");
	Light3TexFogReflectSkinnedTech = fx->GetTechniqueByName("Light3TexFogReflectSkinned");

	Light0TexAlphaClipFogReflectSkinnedTech = fx->GetTechniqueByName("Light0TexAlphaClipFogReflectSkinned");
	Light1TexAlphaClipFogReflectSkinnedTech = fx->GetTechniqueByName("Light1TexAlphaClipFogReflectSkinned");
	Light2TexAlphaClipFogReflectSkinnedTech = fx->GetTechniqueByName("Light2TexAlphaClipFogReflectSkinned");
	Light3TexAlphaClipFogReflectSkinnedTech = fx->GetTechniqueByName("Light3TexAlphaClipFogReflectSkinned");

	WorldViewProj     = fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	WorldViewProjTex  = fx->GetVariableByName("gWorldViewProjTex")->AsMatrix();
	World             = fx->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	BoneTransforms    = fx->GetVariableByName("gBoneTransforms")->AsMatrix();
	//ShadowTransform   = fx->GetVariableByName("gShadowTransform")->AsMatrix();
	TexTransform      = fx->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = fx->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = fx->GetVariableByName("gFogColor")->AsVector();
	FogStart          = fx->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = fx->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = fx->GetVariableByName("gDirLights");
	Mat               = fx->GetVariableByName("gMaterial");
	DiffuseMap        = fx->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap           = fx->GetVariableByName("gCubeMap")->AsShaderResource();
	NormalMap         = fx->GetVariableByName("gNormalMap")->AsShaderResource();
	//ShadowMap         = fx->GetVariableByName("gShadowMap")->AsShaderResource();
	//SsaoMap           = fx->GetVariableByName("gSsaoMap")->AsShaderResource();
}

NormalMapEffect::~NormalMapEffect()
{
}