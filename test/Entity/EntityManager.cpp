//
// Created by admin on 2026/9/10.
//

#include "EntityManager.h"
#include "Entity.h"
#include "Entity_pillar.h"


std::shared_ptr<Entity> EntityManager::CreateEntity(std::string name) {
    std::shared_ptr<Entity> entity;
    if(name == "Entity") {
        entity = std::make_shared<Entity>();
    }else if(name == "Entity_pillar"){
        entity = std::make_shared<Entity_pillar>();




    }else {
        entity = std::make_shared<Entity>();
    }
    mEntity.push_back(entity);

    return entity;
}

void EntityManager::Draw() {
    for(auto& entity : mEntity) {
        entity->Draw();
    }
}

void EntityManager::Append(std::shared_ptr<Entity> entity) {
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
