//
// Created by admin on 2026/9/14.
//

#pragma once
#include "Entity.h"


class Entity_skybox :public Entity
{
public:
    Entity_skybox();
    void Update(float deltaTime) override;
    virtual void Draw() override;// 绘制

};

