//
// Created by admin on 2026/9/10.
//

#include "EntityManager.h"
#include "Entity.h"
#include "Entity_pillar.h"
#include "Global.h"
#include "Core/Camera.h"

bool IsOutsidePlane(Plane plane,std::vector<XMFLOAT3>& points)
{
    for (const auto& p : points)
    {
        float distance = plane.Distance(p);
        // 有一个点在平面内侧
        if (distance <= 0.0f)
        {
            return false;
        }
    }
    // 所有点都在外侧
    return true;
}

void EntityManager::Draw() {
    for(auto& entity : mEntity) {
        //绘制实体时做剔除
        // if (IsOutsidePlane(Global::camera->plane1,entity->WorldOBBvertex) || IsOutsidePlane(Global::camera->plane2,entity->WorldOBBvertex)) {
        // }else {
        //     entity->Draw();
        // }
        entity->Draw();
    }
}

void EntityManager::Append(std::shared_ptr<Entity> entity) {
    mEntity.push_back(entity);
}

void EntityManager::Update(float deltaTime) {
    auto sprites = mEntity;
    for(auto& sprite : sprites)
    {
        sprite->Update(deltaTime);
    }
    // ClearList();//清理
}

void EntityManager::ClearList() {
    mEntity.erase(
        std::remove_if(mEntity.begin(),mEntity.end(),
            [](auto _sprite)
            {
                return !_sprite->isdraw;
            }
        ),
        mEntity.end()
    );
}

std::shared_ptr<Entity> EntityManager::GetSharedPtr(Entity* _unit) {
    for (auto& unit : mEntity)
    {
        if (unit.get() == _unit)
        {
            return unit;
        }
    }
    return nullptr;
}
