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
    float tMin = 0;
    float tMax = FLT_MAX;

    // =========================
    // X 轴
    // =========================
    if (std::abs(rayDir.x) < 1e-6f)
    {
        // 射线在 X 方向没有移动
        if (rayPos.x < boxMin.x || rayPos.x > boxMax.x)
            return -1;
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
            return -1;
    }

    // =========================
    // Y 轴
    // =========================
    if (std::abs(rayDir.y) < 1e-6f)
    {
        if (rayPos.y < boxMin.y || rayPos.y > boxMax.y)
            return -1;
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
            return -1;
    }

    // =========================
    // Z 轴
    // =========================
    if (std::abs(rayDir.z) < 1e-6f)
    {
        if (rayPos.z < boxMin.z || rayPos.z > boxMax.z)
            return -1;
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
            return -1;
    }

    // tMin = 射线进入 AABB 的位置
    return tMin;
}

float RayIntersectTriangle(XMFLOAT3& rayOrigin,XMFLOAT3& rayDirection,XMFLOAT3& v0,XMFLOAT3& v1,XMFLOAT3& v2)
{
    XMVECTOR O = XMLoadFloat3(&rayOrigin);
    XMVECTOR D = XMVector3Normalize(XMLoadFloat3(&rayDirection));

    XMVECTOR V0 = XMLoadFloat3(&v0);
    XMVECTOR V1 = XMLoadFloat3(&v1);
    XMVECTOR V2 = XMLoadFloat3(&v2);

    XMVECTOR edge1 = V1 - V0;
    XMVECTOR edge2 = V2 - V0;

    XMVECTOR pvec = XMVector3Cross(D, edge2);//垂直于edge2和射线D的向量

    float det = XMVectorGetX(XMVector3Dot(edge1, pvec));//pvec方向分量

    const float EPSILON = 1e-6f;

    // 射线与三角形平行
    if (fabs(det) < EPSILON)return -1;

    float invDet = 1.0f / det;

    // 计算 u
    XMVECTOR tvec = O - V0;

    float u = XMVectorGetX(XMVector3Dot(tvec, pvec)) * invDet;

    if (u < -EPSILON || u > 1.0f+ EPSILON)return -1;

    // 计算 v
    XMVECTOR qvec = XMVector3Cross(tvec, edge1);

    float v = XMVectorGetX(XMVector3Dot(D, qvec)) * invDet;

    if (v < -EPSILON || u + v > 1.0f+ EPSILON)return -1;

    // 计算射线参数 t
    float t = XMVectorGetX(XMVector3Dot(edge2, qvec)) * invDet;

    // 交点在射线起点后方
    if (t < 0.0f)return -1;

    return t;
}

float RayIntersectTriangleModel(XMFLOAT3 &rayOrigin, XMFLOAT3 &rayDirection, Entity* _entity) {
    float closestDistance = FLT_MAX;

    XMMATRIX worldMatrix = _entity->GetWorldMatrix();

    XMVECTOR O = XMLoadFloat3(&rayOrigin);
    XMVECTOR D = XMVector3Normalize(
        XMLoadFloat3(&rayDirection)
    );

    for (size_t i = 0;
         i + 2 < _entity->meshFbx.m_indices.size();
         i += 3)
    {
        uint32_t index0 = _entity->meshFbx.m_indices[i];
        uint32_t index1 = _entity->meshFbx.m_indices[i + 1];
        uint32_t index2 = _entity->meshFbx.m_indices[i + 2];

        XMVECTOR V0 = XMLoadFloat3(
            &_entity->meshFbx.m_vertices[index0].position);

        XMVECTOR V1 = XMLoadFloat3(
            &_entity->meshFbx.m_vertices[index1].position);

        XMVECTOR V2 = XMLoadFloat3(
            &_entity->meshFbx.m_vertices[index2].position);

        // Local → World
        V0 = XMVector3TransformCoord(V0, worldMatrix);
        V1 = XMVector3TransformCoord(V1, worldMatrix);
        V2 = XMVector3TransformCoord(V2, worldMatrix);

        XMFLOAT3 v0, v1, v2;

        XMStoreFloat3(&v0, V0);
        XMStoreFloat3(&v1, V1);
        XMStoreFloat3(&v2, V2);

        float distance = RayIntersectTriangle(
            rayOrigin,
            rayDirection,
            v0,
            v1,
            v2
        );

        if (distance >= 0.0f &&
            distance < closestDistance)
        {
            closestDistance = distance;
        }
    }

    if (closestDistance == FLT_MAX)
        return -1;

    return closestDistance;
}




// float RayIntersectTriangleModel(XMFLOAT3 &rayOrigin, XMFLOAT3 &rayDirection, Entity* _entity) {
//     float closestDistance = FLT_MAX;
//
//
//
//     // 每 3 个 index 构成一个三角形
//     for (int i = 0; i + 2 < _mesh->m_indices.size(); i += 3)
//     {
//         uint32_t index0 = _mesh->m_indices[i];
//         uint32_t index1 = _mesh->m_indices[i + 1];
//         uint32_t index2 = _mesh->m_indices[i + 2];
//
//         XMFLOAT3& v0 = _mesh->m_vertices[index0].position;
//         XMFLOAT3& v1 = _mesh->m_vertices[index1].position;
//         XMFLOAT3& v2 = _mesh->m_vertices[index2].position;
//
//         float distance = RayIntersectTriangle(
//             rayOrigin,
//             rayDirection,
//             v0,
//             v1,
//             v2
//         );
//
//         // RayIntersectTriangle 约定：
//         // < 0 表示没有相交
//         // >= 0 表示相交距离
//         if (distance > 0 && distance < closestDistance)
//         {
//             closestDistance = distance;
//         }
//     }
//
//     if (closestDistance == FLT_MAX)
//     {
//         return 0;
//     }
//
//     return closestDistance;
// }