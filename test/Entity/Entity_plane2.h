//
// Created by admin on 2026/10/12.
//

#pragma once
#include "Entity.h"


class Entity_plane2 :public Entity
{
public:
    Entity_plane2();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
};
