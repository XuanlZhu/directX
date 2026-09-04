//
// Created by admin on 2026/9/2.
//

#pragma once
#include "CSprite.h"


class Sprite_claw : public CSprite
{
public:
    Sprite_claw();
    void Update(float deltaTime) override;
    void ThrowOut() override;
    virtual void Draw();// 绘制

    float maxTime = 2;
    float startTime = 0;
    int ThrowState = 0;//扔出状态
    float returnTime = 2;//回来时间
    CSprite* CrawledItem = nullptr;//抓取物体
    CVector2 offset;//偏移
    float swingTime=0;//摆动时间

};
