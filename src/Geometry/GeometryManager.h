#pragma once

#include"MeshGeometry.h"
#include"LoadObject.h"
#include"../Resources/FrameResource.h"

class GeometryManager
{
public:
	GeometryManager(Device* device, ID3D12GraphicsCommandList* cmdList);
	~GeometryManager();
	void CreateMesh(std::string name,std::vector<Vertex> vertices, std::vector<std::uint16_t> indices);
	void CreateSubMesh(std::string name, std::string subName, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void CreateMeshVertexUpload(std::string name, UINT vbSize, std::vector<std::uint16_t> indices);
	MeshGeometry* GetGeo(std::string name) { return m_Geometries[name].get(); }
	void LoadMesh(std::string fileName,std::string geoName,std::string subName);
private:
	//����ӳ�����
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> m_Geometries;
	Device* m_Device;
	ID3D12GraphicsCommandList* m_CmdList;
	LoadObject lo;
};

