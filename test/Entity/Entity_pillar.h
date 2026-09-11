//
// Created by admin on 2026/9/10.
//

#pragma once
#include "Entity.h"


class Entity_pillar :public Entity
{
public:
    Entity_pillar();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
};
