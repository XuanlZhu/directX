//
// Created by admin on 2026/9/10.
//

#include "Entity_pillar.h"

#include "Global.h"
#include "Mesh/Mesh_pillar.h"

Entity_pillar::Entity_pillar() {
    mesh = Mesh_pillar();
}

void Entity_pillar::Update(float deltaTime) {

}

void Entity_pillar::Draw() {
    Entity::Draw();
    auto pos = WorldToScreen(position);
    Global::graphic->DrawText2("zhuzi", pos.x, pos.y);
}
