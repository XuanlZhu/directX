//
// Created by admin on 2026/9/28.
//

#pragma once
#include <DirectXMath.h>

#include "Entity/Entity.h"
class Entity_plane;
class CModel;
using namespace DirectX;

class TerrainNode
{
public:
    Entity_plane* entity = nullptr;
    // 世界空间范围
    XMFLOAT2 min; // 左下角(x,z)
    XMFLOAT2 max; // 右上角(x,z)
    // 当前LOD等级
    int lodLevel=0;
    // 子节点
    std::unique_ptr<TerrainNode> children[4];
    TerrainNode* father = nullptr;

    bool isLeaf = false;
    // 这个区域对应的mesh
    CModel mesh;
    //分割
    void Separate(int level);
    void Merge(int level);
    void Draw();
    void SetLeaf();
    void UpdateLOD();
    float GetMinDistance();
    float GetMaxDistance();
    void PrintNodes();
    int GetLOD(float distance);
};
