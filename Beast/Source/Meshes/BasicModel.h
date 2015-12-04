#ifndef BASICMODEL_H
#define BASICMODEL_H

#include "MeshGeometry.h"
#include "..\TextureMgr.h"
#include "..\Vertex.h"
#include "..\Structs.h"
#include "../Camera.h"
#include "../Objects/Skybox.h"
#include "../Collider.h"

class BasicModel
{
public:
	BasicModel();

	BasicModel(ID3D11Device* device, TextureMgr& texMgr, std::string& modelFilename, const std::wstring& texturePath, bool bump);
	~BasicModel();

	UINT SubsetCount;

	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	std::vector<ID3D11ShaderResourceView*> NormalMapSRV;

	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormalTexTan> VerticesBump;
	std::vector<Vertex::Basic> VerticesSimple;
	std::vector<USHORT> Indices;
	std::vector<MeshGeometry::Subset> Subsets;

	MeshGeometry ModelMesh;
	bool bump;
};

struct BasicModelInstance
{
	BasicModel* Model;
	XMFLOAT4X4 World;

	XMFLOAT3 position;
	XMFLOAT3 up;
	XMFLOAT3 fwd;
	XMFLOAT3 right;
	XMFLOAT4X4 rot;
	XMFLOAT3 scale;

	void Walk(float dt, Camera* cam/*, Terrain* ter*/);
	void Strafe(float dt, Camera* cam/*, Terrain* ter*/);
	void RotateY(float angle);
	void Update(float dt);
	void Draw(ID3D11Device *device, ID3D11DeviceContext *context, Camera *cam, DirectionalLight dirLights[3], Skybox *sky);
	XMFLOAT3 GetPos();
};

#endif // SKINNEDMODEL_H