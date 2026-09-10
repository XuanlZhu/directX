//
// Created by admin on 2026/9/10.
//

#include "EntityManager.h"

#include "Entity.h"

void EntityManager::Draw() {
}

void EntityManager::Append(std::shared_ptr<Entity> entity) {
}

void EntityManager::Update(float deltaTime) {
    auto sprites = mEntity;
    for(auto sprite : sprites)
    {
        if(sprite)
        {
            sprite->Update(deltaTime);
        }
    }
    ClearList();//清理
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
