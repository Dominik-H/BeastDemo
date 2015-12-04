#pragma once

#include "Level.h"
#include "Texture.h"

class Level0 :
	public Level
{
public:
	Level0();
	~Level0();

	virtual bool LoadLevel(ID3D11Device *device, ID3D11DeviceContext *context);
	virtual void Update(float dt);
	virtual void Draw(ID3D11Device *device, ID3D11DeviceContext *context);

private:
	Texture tex;
	Texture norm;
	Material gridMat;
};

