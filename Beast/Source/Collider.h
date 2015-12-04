#pragma once

#include "Helper.h"
#include "xnacollision.h"
#include "Vertex.h"
#include "Meshes\MeshGeometry.h"

struct CollisionBox
{
	void CreateBuffers(ID3D11Device *device);

	XNA::OrientedBox OBB;
	std::vector<CollisionBox *> with;

	ID3D11Buffer *VB;
	ID3D11Buffer *IB;
};

class Collider
{
public:
	Collider();
	~Collider();

	void CreateStatic(std::vector<Vertex::PosNormalTexTan> &inVert, const std::vector<USHORT> &inInd, const std::vector<MeshGeometry::Subset> &inSubs);
	void CreateStatic(std::vector<Vertex::Basic> &inVert);
	void CreateAnimated(std::vector<Vertex::PosNormalTexTanSkinned> &in);

	void CreateStaticD(ID3D11Device *device, std::vector<Vertex::PosNormalTexTan> &inVert, const std::vector<USHORT> &inInd, const std::vector<MeshGeometry::Subset> &inSubs);
	void CreateStaticD(ID3D11Device *device, std::vector<Vertex::Basic> &inVert);
	void CreateAnimatedD(ID3D11Device *device, std::vector<Vertex::PosNormalTexTanSkinned> &in);

	std::vector<std::pair<int, CollisionBox>> OBBs;
};

