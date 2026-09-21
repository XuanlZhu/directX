//
// Created by admin on 2026/9/21.
//

#pragma once
#include "Entity.h"


class Entity_player :public Entity
{
public:
    Entity_player();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制
};
