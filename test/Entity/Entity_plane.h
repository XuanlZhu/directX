//
// Created by admin on 2026/9/15.
//

#pragma once
#include "Entity.h"


class Entity_plane :public Entity
{
public:
    Entity_plane();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
    void readHightMap(std::string path);
    void ApplyHeightMap();

    // ID3D11ShaderResourceView* m_hightMap = nullptr;//高度图
    std::vector<std::vector<float>> m_heightData;//高度图
};
