//
// Created by admin on 2026/9/1.
//

#include "Global.h"

#include <chrono>
#include <iostream>
#include <random>

#include "Core/Camera.h"
#include "Core/CImageManager.h"
#include "Core/Game.h"
#include "Core/Graphic.h"
#include "Core/SpriteList.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity_ball.h"
#include "Entity/Entity_pillar.h"
#include "Entity/Entity_plane.h"
#include "Entity/Entity_player.h"
#include "Entity/Entity_skybox.h"
#include "Entity/Entity_water.h"
#include "Sprites/Sprite_claw.h"
#include "Sprites/Sprite_gold.h"
#undef min
#undef max

int RandomInt(int _min, int _max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(_min,_max);
    return dis(gen);
}

float RandomFloat(float _min, float _max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    uniform_int_distribution<> dis(_min,_max);
    return dis(gen);
}

std::wstring StringToWString(std::string str) {
    int size_needed = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring wstr(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        wstr.data(),
        size_needed
    );

    return wstr;
}

float GetNowTime() {
    auto ts = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(ts-Global::game->mStartTime).count();
}

std::weak_ptr<CSprite> CreateCSprite(std::string _sprite, XMFLOAT2 _pos) {
    std::shared_ptr<CSprite> sprite;;//创建精灵类，下发到当前精灵表
    if (_sprite=="Sprite_claw") {
        sprite = std::make_shared<Sprite_claw>();
    }else if( _sprite=="Sprite_gold") {
        sprite = std::make_shared<Sprite_gold>();
    }else {
        sprite = std::make_shared<CSprite>();
    }

    sprite->SetPosition(_pos);
    std::cout << "CreateCSprite,加入精灵表" << std::endl;
    Global::spriteList->Append(sprite);
    sprite->OnCreated();
    return sprite;
}

void DrawTexture(std::string name, float x, float y, float width, float height, float rotatX, float rotatY,float angleDeg) {
    auto img = Global::imageManager->GetImage(name);
    Global::graphic->DrawTexture2(img,x,y,width,height,rotatX,rotatY,angleDeg);
}

void DrawLine(CVector2 startPos, CVector2 endPos, XMFLOAT3 color) {
    Global::graphic->DrawLine2(XMFLOAT2(startPos.x,startPos.y), XMFLOAT2(endPos.x,endPos.y), color);
}

void DrawLine(XMFLOAT2 startPos, XMFLOAT2 endPos, XMFLOAT3 color) {
    Global::graphic->DrawLine2(startPos, endPos, color);
}

std::shared_ptr<Entity> CreateEntity(std::string name,XMFLOAT3 pos) {
    std::shared_ptr<Entity> entity;
    if(name == "Entity") {
        entity = std::make_shared<Entity>();
    }else if(name == "Entity_pillar"){
        entity = std::make_shared<Entity_pillar>();
    }else if(name == "Entity_skybox"){
        entity = std::make_shared<Entity_skybox>();
    }else if(name == "Entity_plane"){
        entity = std::make_shared<Entity_plane>();
    }else if(name == "Entity_ball"){
        entity = std::make_shared<Entity_ball>();
    }else if(name == "Entity_water"){
        entity = std::make_shared<Entity_water>();
    }else if(name == "Entity_player"){
        entity = std::make_shared<Entity_player>();


    }else {
        entity = std::make_shared<Entity>();
    }
    entity->SetPosition(pos);//设置位置
    Global::entityManager->Append(entity);//添加到实体管理器
    return entity;
}

XMFLOAT2 WorldToScreen(XMFLOAT3 _pos) {
    XMVECTOR pos = XMLoadFloat3(&_pos);
    XMVECTOR screenPos = XMVector3Project(
        pos,
        0.0f, 0.0f,
        800.0f, 600.0f,     // 屏幕区域
        0.0f, 1.0f,         // 深度范围
        Global::graphic->m_projection,
        Global::camera->GetViewMatrix(),
        XMMatrixIdentity()
    );

    XMFLOAT3 result;
    XMStoreFloat3(&result, screenPos);

    return {result.x,result.y};
}

float RayIntersectAABB(XMFLOAT3 &rayPos, XMFLOAT3 &rayDir, XMFLOAT3 &boxMin, XMFLOAT3 &boxMax) {
    float tMin = 0.0f;
    float tMax = FLT_MAX;

    // X 轴
    if (std::abs(rayDir.x) < 1e-6f)
    {
        // 射线平行于 X 轴
        if (rayPos.x < boxMin.x || rayPos.x > boxMax.x)
            return false;
    }
    else
    {
        float t1 = (boxMin.x - rayPos.x) / rayDir.x;
        float t2 = (boxMax.x - rayPos.x) / rayDir.x;

        if (t1 > t2)
            std::swap(t1, t2);

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    // Y 轴
    if (std::abs(rayDir.y) < 1e-6f)
    {
        if (rayPos.y < boxMin.y || rayPos.y > boxMax.y)
            return false;
    }
    else
    {
        float t1 = (boxMin.y - rayPos.y) / rayDir.y;
        float t2 = (boxMax.y - rayPos.y) / rayDir.y;

        if (t1 > t2)
            std::swap(t1, t2);

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    // Z 轴
    if (std::abs(rayDir.z) < 1e-6f)
    {
        if (rayPos.z < boxMin.z || rayPos.z > boxMax.z)
            return false;
    }
    else
    {
        float t1 = (boxMin.z - rayPos.z) / rayDir.z;
        float t2 = (boxMax.z - rayPos.z) / rayDir.z;

        if (t1 > t2)
            std::swap(t1, t2);

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    return true;
}

bool RayIntersectTriangle(XMFLOAT3 pos, XMFLOAT3 dir) {
    //先做AABB粗略检测
    for (auto x:Global::entityManager->mEntity) {
        if (true) {//AABB检测
            //加速结构BVH
            if (true) {//精确检测
                return true;
            }
        }
    }
}