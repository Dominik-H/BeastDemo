/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is Basic Effect source that links Basic Effect shader with cpp code.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "BasicEffect.h"

BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
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

	WorldViewProj     = fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	WorldViewProjTex  = fx->GetVariableByName("gWorldViewProjTex")->AsMatrix();
	World             = fx->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = fx->GetVariableByName("gTexTransform")->AsMatrix();
	ShadowTransform   = fx->GetVariableByName("gShadowTransform")->AsMatrix();
	EyePosW           = fx->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = fx->GetVariableByName("gFogColor")->AsVector();
	FogStart          = fx->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = fx->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = fx->GetVariableByName("gDirLights");
	Mat               = fx->GetVariableByName("gMaterial");
	DiffuseMap        = fx->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap           = fx->GetVariableByName("gCubeMap")->AsShaderResource();
	ShadowMap         = fx->GetVariableByName("gShadowMap")->AsShaderResource();
	SsaoMap           = fx->GetVariableByName("gSsaoMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}