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
#include "Entity/Entity_pillar.h"
#include "Sprites/Sprite_claw.h"
#include "Sprites/Sprite_gold.h"

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
