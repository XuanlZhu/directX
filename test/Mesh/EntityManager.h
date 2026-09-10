//
// Created by admin on 2026/9/10.
//

#pragma once
#include <memory>
#include <vector>


class Entity;

class EntityManager {
public:
    void Draw();//绘制
    void Append(std::shared_ptr<Entity> entity);//添加实体
    void Update(float deltaTime);//更新
    void ClearList();//清理表
    std::shared_ptr<Entity> GetSharedPtr(Entity* unit);

    std::vector<std::shared_ptr<Entity>> mEntity;
};
