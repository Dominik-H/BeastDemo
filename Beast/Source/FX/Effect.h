/*************************************************************************************
* File: Init.h; Date of creation: 10.Feb.2015; Date of Last Modification: 10.Feb.2015
*
* This is a header file for Effect Base class that creates effect.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef EFFECT_H
#define EFFECT_H

#include "../Helper.h"
#include "../Structs.h"

class Effect
{
public:
	Effect( ID3D11Device* device, const std::wstring& filename );
	virtual ~Effect( void );

protected:
	ID3DX11Effect* fx;
};

#endif // EFFECT_H