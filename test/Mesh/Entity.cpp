//
// Created by admin on 2026/9/10.
//

#include "Entity.h"

Entity::Entity() {

}

void Entity::Draw() {
}

void Entity::Update(float deltaTime) {

}

XMMATRIX Entity::GetWorldMatrix() {
    // 缩放
    XMMATRIX scaleX = XMMatrixScaling(
        scale.x,
        scale.y,
        scale.z
    );
    // 旋转
    XMMATRIX rotationX = XMMatrixRotationRollPitchYaw(
        XMConvertToRadians(rotation.x),
        XMConvertToRadians(rotation.y),
        XMConvertToRadians(rotation.z)
    );
    // 位移
    XMMATRIX translation = XMMatrixTranslation(
        position.x,
        position.y,
        position.z
    );
    // World = Scale * Rotation * Translation
    return scaleX * rotationX * translation;
}

void Entity::Rotate(float x, float y, float z) {
}

XMFLOAT3 Entity::Getfacing() {
    XMVECTOR defaultFacing = XMLoadFloat3(&facing);
    XMVECTOR result = XMVector3TransformNormal(defaultFacing, GetWorldMatrix());

    XMFLOAT3 resultFloat;
    XMStoreFloat3(&resultFloat, XMVector3Normalize(result));

    return resultFloat;
}

void Entity::Move(bool forward) {
}
