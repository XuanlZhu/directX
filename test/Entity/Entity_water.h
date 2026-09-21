//
// Created by admin on 2026/9/20.
//

#pragma once
#include "Entity.h"
#include "Core/Camera.h"

class Entity_water :public Entity
{
public:
    Entity_water();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
    Plane plane1;

    std::vector<std::vector<float>> m_heightData;//高度图
};
