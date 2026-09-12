//
// Created by admin on 2026/9/11.
//

#pragma once
#include <string>
#include <vector>

#include "assimp/scene.h"
#include "Core/Graphic.h"


class CModel {
public:
    CModel();
    CModel(std::string _path);
    void Load(std::string _path);
    void Draw();
    void Print();

    void ProcessNode(aiNode* _node, const aiScene* _scene);//遍历 FBX 内部的“节点树”，找到每一个 Mesh
    void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);//把一个 Mesh 转换成 Vertex + Index 数据

    std::vector<Vertex3fbx> m_vertices;
    std::vector<uint32_t> m_indices;
};
