//
// Created by admin on 2026/10/12.
//

#pragma once
#include "Entity.h"


class Entity_cylinder :public Entity
{
public:
    Entity_cylinder();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
};
