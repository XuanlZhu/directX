//
// Created by admin on 2026/9/19.
//

#pragma once
#include "Entity.h"


class Entity_ball :public Entity
{
public:
    Entity_ball();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制

    std::vector<Vertex3> VerticesLocalBox;//本地包围盒,使用本地坐标
    std::vector<Vertex3> WorldAABB;//世界AABB，使用世界坐标
};
