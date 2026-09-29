//
// Created by admin on 2026/9/15.
//

#pragma once
#include <queue>

#include "Entity.h"
#include "Mesh/TerrainNode.h"


class Entity_plane :public Entity
{
public:
    Entity_plane();
    void Update(float deltaTime) override;
    void Draw() override;// 绘制
    void readHightMap(std::string path);
    void ApplyHeightMap();

    int m_size = 50;

    // ID3D11ShaderResourceView* m_hightMap = nullptr;//高度图
    std::vector<std::vector<float>> m_heightData;//高度图

    void ExecutionQueue();
    std::queue<TerrainNode*> queue;
    TerrainNode root;
    D3D11_PRIMITIVE_TOPOLOGY priType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    void SwitchPri();
};
