//
// Created by admin on 2026/9/2.
//

#include "Sprite_claw.h"

Sprite_claw::Sprite_claw() {
    pngName = "claw";
    mWidth = 32;
    mHeight = 19;
    mPosInit = CVector2(365+16,120);
    angleDeg = 0;
    mSpeed = 300;//速度


    oWidth = 32;//原始宽高
    oHeight = 19;//原始宽高
    maxTime = 1.5;//扔出时间
    ThrowState = 0;//扔出状态
    XAnchor = XMFLOAT2(0.5,0);
}

void Sprite_claw::Update(float deltaTime) {
    // CSprite::Update(deltaTime);
    //扔出状态
    if(ThrowState==1) {
        GetWorldVertices();//计算世界顶点
        GetBoxVertices();//计算包围盒顶点
        auto it = CollisionDetection();//碰撞检测

        if (GetNowTime()-startTime>maxTime || it) {
            //抓住了
            if (it){CrawledItem=it;offset=it->mPos-mPos;}
            ThrowState = 2;
            returnTime = GetNowTime()-startTime;
            startTime = GetNowTime();
        }
    }
    //回来状态
    if(GetNowTime()-startTime>returnTime && ThrowState==2) {
        ThrowState = 0;
        if (CrawledItem) {
            CrawledItem->Destroy();
            CrawledItem = nullptr;
        }
    }

    //设置钩爪摆动
    if (ThrowState == 1) {
        mPos = mPos + dir.Normalize()*mSpeed*deltaTime;//过去
    }else if (ThrowState == 2) {
        mPos = mPos - dir.Normalize()*mSpeed*deltaTime;//回来
    }else{
        //钩爪摆动
        swingTime += deltaTime;
        float speed = 2.0f;// 摆动速度
        float maxAngle = 45.0f;// 最大角度
        float angle = sin(swingTime * speed)*maxAngle;

        angleDeg = angle;
        mPos = mPosInit.Rotate(CVector2(378,77),angle);
    }
    // float speed = 2.0f;// 摆动速度
    // float maxAngle = 45.0f;// 最大角度
    // float angle = sin(swingTime * speed)*maxAngle;
    // DrawTexture("line",378,77,5,50,378,77,angle);

    //如果有抓取物
    if (CrawledItem) {
        CrawledItem->SetPosition(mPos+offset);
    }
}

void Sprite_claw::ThrowOut() {
    if (ThrowState==0) {
        dir = (mPos - CVector2(378,77)).Normalize();
        ThrowState = 1;
        startTime = GetNowTime();
    }
}

void Sprite_claw::Draw() {
    CSprite::Draw();
    float speed = 2.0f;// 摆动速度
    float maxAngle = 45.0f;// 最大角度
    float angle = sin(swingTime * speed)*maxAngle;
    // angle = 0;

    float length = (mPos-CVector2(378,77)).Length();
    DrawTexture("line",378,77,5,length,378,77,angle);
}
